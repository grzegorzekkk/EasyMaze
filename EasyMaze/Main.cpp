#include"Maze.h"
#include"Display.h"
#include"Player.h"
#include<SDL.h>
#include<iostream>
#include<string>
#include<ctime>

using namespace std;

int main(int argc, char* args[])
{
	srand(time(NULL));

	//Main game loop
	bool gameloop = false;
	//Main application loop
	bool apploop = true;
	//Maze size
	int mazeSize;

	//Setup and display
	Utils setup;
	Display display(640, 480, "Easy Maze", &setup);

	//Event handler
	SDL_Event e;

	while (apploop)
	{
		display.showMenu(e);
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				gameloop = false;
				apploop = false;
			}
			//users choose option
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1:
					mazeSize = display.sizePrompt(e);
					if(mazeSize>0)
						gameloop = true;
					break;
				case SDLK_2:
					while (display.showRules(e))
						;
					break;
				case SDLK_3:
					while (display.showCredits(e))
						;
					break;
				}
			}
		}

		//while game is running
		while (gameloop)
		{
			Maze maze(mazeSize);
			maze.generate();

			display.loadMaze(&maze);
			Player p(&setup, &display.tiles);

			int x = p.getStartxy();
			//Level camera
			SDL_Rect camera = { x, x, 640, 480 };

			bool playing = true;
			time_t start, stop;
			double czas;

			//start game timer
			time(&start);
			//While player is playing
			while (playing)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						gameloop = false;
						playing = false;
						apploop = false;
					}
					//Handle input for the player
					p.handleEvent(e);
				}
				//Move the player
				p.move();
				p.setCamera(camera);
				//Clear screen
				SDL_SetRenderDrawColor(setup.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(setup.renderer);
				//Render level
				for (int i = 0; i < setup.TOTAL_TILES; ++i)
				{
					display.tiles[i]->render(camera);
				}
				//Render player
				p.render(camera);
				//Update screen
				SDL_RenderPresent(setup.renderer);
				//Check if player ended level
				if (p.touchesExit(p.getPlayerBox()))
				{
					//End game timer
					time(&stop);
					czas = difftime(stop, start);

					//Exit loops
					gameloop = false;
					playing = false;
					//Show ending info
					while (display.showSummary(e, czas))
						;
				}
			}
		}
	}


	//free resource and close window
	display.close();

	return 0;
}