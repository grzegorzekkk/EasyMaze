#ifndef TILE_H
#define TILE_H

#include<SDL.h>
#include"Utils.h"

class Tile
{
public:
	//Initializes position and type
	Tile();
	Tile(int x, int y, int tileType, Utils *config);

	bool wall = true;
	bool exit = false;
	bool start = false;

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

	static bool checkCollision(SDL_Rect a, SDL_Rect b);
private:
	//The attributes of the tile
	SDL_Rect collisionBox;

	//The tile type
	int type;

	//config pointer
	Utils *cfg;
};

#endif
