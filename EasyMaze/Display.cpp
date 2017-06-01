#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include "Display.h"

Display::Display(int width, int height, char *window_name, Utils *config)
	: cfg(config), SCREEN_WIDTH(width), SCREEN_HEIGHT(height)
{
	//synchronize setup info
	config->SCREEN_WIDTH = width;
	config->SCREEN_HEIGHT = height;

	/*This piece of code was originally from Lazy Foo' Productions
	(http://lazyfoo.net/)*/

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		cfg->window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (cfg->window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			cfg->renderer = SDL_CreateRenderer(cfg->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (cfg->renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(cfg->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	loadMedia();
}

Display::~Display()
{
}

bool Display::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Set window icon
	SDL_Surface* windowIcon = IMG_Load("./assets/hero.png");
	SDL_SetWindowIcon(cfg->window, windowIcon);
	SDL_FreeSurface(windowIcon);
	
	//Open the font
	cfg->font = TTF_OpenFont("./assets/Minecraft.ttf", 24);
	if (cfg->font == NULL)
	{
		printf("Failed to load minecraft font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	//Load player texture
	if (!cfg->playerTexture.loadFromFile("./assets/hero.png", cfg->renderer))
	{
		printf("Failed to load player texture!\n");
		success = false;
	}
	//Load tile texture
	if (!cfg->tileTexture.loadFromFile("./assets/tiles.png", cfg->renderer))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}
	//Load background texture
	if (!cfg->menuTexture.loadFromFile("./assets/wall.png", cfg->renderer))
	{
		printf("Failed to load menu texture!\n");
		success = false;
	}

	return success;
}

bool Display::setTiles(Maze *mazeptr)
{
	// 2d maze to 1d array of tiles
	std::vector<Tile> tmp;
	for (int i = 0; i < mazeptr->getSize(); i++) {
		for (int j = 0; j < mazeptr->getSize(); j++) {
			tmp.push_back(mazeptr->maze_map[i][j]);
		}
	}

	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Initialize the tiles
	for (int i = 0; i < cfg->TOTAL_TILES; ++i)
	{
		//Determines what kind of tile will be made
		int tileType = -1;

		if (tmp[i].exit == true)
		{
			tiles.push_back(new Tile(x, y, Utils::TILE_DOOR, cfg));
		}
		else if (tmp[i].wall == true)
		{
			tiles.push_back(new Tile(x, y, Utils::TILE_WALL, cfg));
		}
		else if (tmp[i].wall == false)
		{
			tiles.push_back(new Tile(x, y, Utils::TILE_FLOOR, cfg));
		}
		else
		{
			//Stop loading map
			printf("Error loading map: Invalid tile type at %d!\n", i);
			tilesLoaded = false;
			break;
		}

		//Move to next tile spot
		x += Utils::TILE_WIDTH;

		//If we've gone too far
		if (x >= cfg->LEVEL_WIDTH)
		{
			//Move back
			x = 0;

			//Move to the next row
			y += Utils::TILE_HEIGHT;
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			cfg->tileClips[Utils::TILE_WALL].x = 0;
			cfg->tileClips[Utils::TILE_WALL].y = 0;
			cfg->tileClips[Utils::TILE_WALL].w = Utils::TILE_WIDTH;
			cfg->tileClips[Utils::TILE_WALL].h = Utils::TILE_HEIGHT;

			cfg->tileClips[Utils::TILE_FLOOR].x = 64;
			cfg->tileClips[Utils::TILE_FLOOR].y = 0;
			cfg->tileClips[Utils::TILE_FLOOR].w = Utils::TILE_WIDTH;
			cfg->tileClips[Utils::TILE_FLOOR].h = Utils::TILE_HEIGHT;

			cfg->tileClips[Utils::TILE_DOOR].x = 128;
			cfg->tileClips[Utils::TILE_DOOR].y = 0;
			cfg->tileClips[Utils::TILE_DOOR].w = Utils::TILE_WIDTH;
			cfg->tileClips[Utils::TILE_DOOR].h = Utils::TILE_HEIGHT;
		}
	}

	//If the map was loaded fine
	return tilesLoaded;
}

bool Display::loadMaze(Maze *mazeptr)
{
	//Set total amount of tiles and level size
	cfg->TOTAL_TILES = mazeptr->getSize()*mazeptr->getSize();
	cfg->LEVEL_WIDTH = mazeptr->getSize() * cfg->TILE_WIDTH;
	cfg->LEVEL_HEIGHT = cfg->LEVEL_WIDTH;

	//Free previous tiles data
	if (!tiles.empty())
	{
		//Deallocate tiles
		for (std::vector< Tile* >::iterator it = tiles.begin(); it != tiles.end(); ++it)
		{
			delete (*it);
		}
		tiles.clear();
	}

	//Load tile map
	if (!setTiles(mazeptr))
	{
		printf("Failed to load maze tiles!\n");
		return false;
	}

	return true;
}

bool Display::showMenu(SDL_Event &e)
{
	renderBackground();

	//Show subtitles
	showText("Easy maze", cfg->SCREEN_WIDTH / 3, cfg->SCREEN_HEIGHT / 5);
	showText("1) New game", cfg->SCREEN_WIDTH / 3, cfg->SCREEN_HEIGHT / 2);
	showText("2) Rules", cfg->SCREEN_WIDTH / 3, (cfg->SCREEN_HEIGHT / 2) + 30);
	showText("3) Credits", cfg->SCREEN_WIDTH / 3, (cfg->SCREEN_HEIGHT / 2) + 60);
	showText("Choose by pressing number on your keyboard i.e '1'", 10, cfg->SCREEN_HEIGHT - 30);

	//update screen
	SDL_RenderPresent(cfg->renderer);

	return false;
}

bool Display::showCredits(SDL_Event &e)
{
	renderBackground();

	//Show subtitles
	showText("Author:", cfg->SCREEN_WIDTH / 3, cfg->SCREEN_HEIGHT / 5);
	showText("Grzegorz Zakrzewski", cfg->SCREEN_WIDTH / 6, cfg->SCREEN_HEIGHT / 2);
	showText("grzesiek6_12 [at] interia.pl", cfg->SCREEN_WIDTH / 6, (cfg->SCREEN_HEIGHT / 2) + 30);
	showText("ESC) Back to menu", cfg->SCREEN_WIDTH / 6, (cfg->SCREEN_HEIGHT / 2) + 90);

	//update screen
	SDL_RenderPresent(cfg->renderer);

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			close();
			exit(0);
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return false;
				break;
			}
		}
	}

	return true;
}

bool Display::showSummary(SDL_Event & e, double time)
{
	renderBackground();

	//Show subtitles
	showText("Congratulations!", cfg->SCREEN_WIDTH / 3, cfg->SCREEN_HEIGHT / 5);

	std::stringstream endTimeInfo;
	endTimeInfo << "You have escaped the maze in: " << time << " seconds.";
	showText(endTimeInfo.str(), cfg->SCREEN_WIDTH / 8, cfg->SCREEN_HEIGHT / 2);
	showText("ESC) Back to menu", cfg->SCREEN_WIDTH / 8, (cfg->SCREEN_HEIGHT / 2) + 60);

	//update screen
	SDL_RenderPresent(cfg->renderer);

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			close();
			exit(0);
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return false;
				break;
			}
		}
	}

	return true;
}

bool Display::showRules(SDL_Event & e)
{
	renderBackground();

	//Show subtitles
	showText("Rules of the game", cfg->SCREEN_WIDTH / 3, cfg->SCREEN_HEIGHT / 5);
	showText("1. Move your hero using arrow keys on keyboard", 5, (cfg->SCREEN_HEIGHT / 2) + 0);
	showText("2. Your goal is to exit the maze", 5, (cfg->SCREEN_HEIGHT / 2) + 30);
	showText("ESC) Back to menu", cfg->SCREEN_WIDTH / 6, (cfg->SCREEN_HEIGHT / 2) + 90);

	//update screen
	SDL_RenderPresent(cfg->renderer);

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			close();
			exit(0);
		}
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return false;
				break;
			}
		}
	}
	return true;
}

int Display::sizePrompt(SDL_Event &e)
{
	int inputSize;
	SDL_Color textColor = { 255, 255, 255, 0xFF };

	//The current input text.
	std::string inputText = "3";
	Texture input;
	input.loadFromRenderedText(inputText.c_str(), textColor, cfg->renderer, cfg->font);

	//Enable text input
	SDL_StartTextInput();

	bool quit = false;
	bool escape = false;
	//While application is running
	while (!quit)
	{
		//The rerender text flag
		bool renderText = false;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				close();
				exit(0);
			}
			//Special key input
			else if (e.type == SDL_KEYDOWN)
			{
				//Handle backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
				{
					//lop off character
					inputText.pop_back();
					renderText = true;
				}
				//Handle copy
				else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
				{
					SDL_SetClipboardText(inputText.c_str());
				}
				//Handle paste
				else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
				{
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
				//Handle enter
				else if (e.key.keysym.sym == SDLK_RETURN)
				{
					if (inputSize >= 25 && inputSize <= 100)
					{
						quit = true;
					}
				}
				else if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					inputSize = -1;
					quit = true;
					escape = true;
				}
			}
			//Special text input event
			else if (e.type == SDL_TEXTINPUT)
			{
				//Not copy or pasting
				if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
				{
					//Append character
					inputText += e.text.text;
					renderText = true;
				}
			}
		}

		//Rerender text if needed
		if (renderText)
		{
			//Text is not empty
			if (inputText != "")
			{
				//Render new text
				input.loadFromRenderedText(inputText.c_str(), textColor, cfg->renderer, cfg->font);
			}
			//Text is empty
			else
			{
				//Render space texture
				input.loadFromRenderedText(" ", textColor, cfg->renderer, cfg->font);
			}
		}
		//Clear screen
		SDL_SetRenderDrawColor(cfg->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(cfg->renderer);

		//Render text textures
		renderBackground();
		showText("Enter width and length of maze (n)", cfg->SCREEN_WIDTH / 7, cfg->SCREEN_HEIGHT / 5);
		showText("ENTER) Accept", cfg->SCREEN_WIDTH / 6, (cfg->SCREEN_HEIGHT / 2) + 60);
		showText("ESC) Back to menu", cfg->SCREEN_WIDTH / 6, (cfg->SCREEN_HEIGHT / 2) + 90);
		input.render(cfg->renderer, (SCREEN_WIDTH - input.getWidth()) / 2, cfg->SCREEN_HEIGHT / 2);

		//if user doesnt want to leave view
		if (!escape)
		{
			//
			inputSize = atoi(inputText.c_str());
			if (inputSize < 25 || inputSize > 100)
			{
				SDL_Color textColorRed = { 255, 0, 0, 0xFF };
				showText("Entered number should be in <25,100>!", 5, cfg->SCREEN_HEIGHT - 30, textColorRed);
			}
		}


		//Update screen
		SDL_RenderPresent(cfg->renderer);
	}

	//Disable text input
	SDL_StopTextInput();

	return inputSize;
}

void Display::renderBackground()
{
	//Set starting render points
	int x = 0;
	int y = 0;

	//Clear window
	SDL_SetRenderDrawColor(cfg->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(cfg->renderer);

	//Fill window with tile
	while (y < cfg->SCREEN_HEIGHT)
	{
		cfg->menuTexture.render(cfg->renderer, x, y);
		x += cfg->TILE_WIDTH;
		if (x == cfg->SCREEN_WIDTH)
		{
			x = 0;
			y += cfg->TILE_HEIGHT;
		}
	}

}

void Display::showText(std::string text, int width, int height, SDL_Color color)
{
	Texture textData;
	textData.loadFromRenderedText(text, color, cfg->renderer, cfg->font);
	textData.render(cfg->renderer, width, height);
	textData.free();
}

void Display::close()
{
	if (!tiles.empty())
	{
		//Deallocate tiles
		for (std::vector< Tile* >::iterator it = tiles.begin(); it != tiles.end(); ++it)
		{
			delete (*it);
		}
		tiles.clear();
	}

	//Free loaded images
	cfg->playerTexture.free();
	cfg->tileTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(cfg->renderer);
	SDL_DestroyWindow(cfg->window);
	cfg->window = NULL;
	cfg->renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

#endif
