#ifndef UTILS_H
#define UTILS_H

#include"Texture.h"
#include<SDL.h>
#include<vector>

class Utils 
{
public:
	Utils();
	~Utils();

	static void setMazeSize(int size);

	//Maze size
	static int LAB_SIZE;

	//Screen dimensions
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	//The dimensions of the level
	int LEVEL_WIDTH;
	int LEVEL_HEIGHT;

	//Tile constants
	static const int TILE_WIDTH = 64;
	static const int TILE_HEIGHT = 64;
	int TOTAL_TILES;

	//The different tile sprites
	static const int TILE_WALL = 0;
	static const int TILE_FLOOR = 1;
	static const int TILE_DOOR = 2;

	Texture tileTexture;
	Texture playerTexture;
	Texture menuTexture;
	SDL_Rect tileClips[3];
	TTF_Font *font = NULL;
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Surface* PNGSurface;
	SDL_Renderer* renderer;
};

#endif

