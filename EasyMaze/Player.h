#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include "Tile.h"
#include "Maze.h"

class Player
{
public:
	//Initializes the variables
	Player(Utils *config, std::vector<Tile*> *allTiles);
	~Player();

	//The dimensions of the player
	static const int PLAYER_WIDTH = 55;
	static const int PLAYER_HEIGHT = 50;

	//Maximum axis velocity of the dot
	static const int PLAYER_VEL = 3;
	static const int ROTATION_SPEED = 5;

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and check collision against tiles
	void move();

	//Centers the camera over the dot
	void setCamera(SDL_Rect& camera);

	//Shows the dot on the screen
	void render(SDL_Rect& camera);

	//Rotates player depending on move
	void rotatePosition(int dir);

	//Check if box collides with wall
	bool touchesWall(SDL_Rect box);

	//Check if player went to exit
	bool touchesExit(SDL_Rect box);

	//Get player's start coordinates
	int getStartxy();

	//Get player box
	SDL_Rect getPlayerBox();
private:
	//Collision box of the dot
	SDL_Rect hitBox;

	//The velocity of the dot
	int mVelX, mVelY;
	int degrees;

	//Pointer to configuration
	Utils *cfg;
	//Pointer to all tiles data
	std::vector<Tile*> *tiles;
};

#endif