#ifndef TILE_CPP
#define TILE_CPP

#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(int x, int y, int tileType, Utils *config)
{
	cfg = config;

	//Get the offsets
	collisionBox.x = x;
	collisionBox.y = y;

	//Set the collision box
	collisionBox.w = Utils::TILE_WIDTH;
	collisionBox.h = Utils::TILE_HEIGHT;

	//Get the tile type
	type = tileType;
}

void Tile::render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, collisionBox))
	{
		//Show the tile
		cfg->tileTexture.render(cfg->renderer, collisionBox.x - camera.x, collisionBox.y - camera.y, &cfg->tileClips[type]);
	}
}

int Tile::getType()
{
	return type;
}

SDL_Rect Tile::getBox()
{
	return collisionBox;
}

bool Tile::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

#endif
