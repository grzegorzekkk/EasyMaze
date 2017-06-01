#ifndef PLAYER_CPP
#define PLAYER_CPP

#include "Player.h"

Player::Player(Utils *config, std::vector<Tile*> *allTiles) : cfg(config), tiles(allTiles)
{
	//Initialize the collision box
	hitBox.x = (config->LAB_SIZE/2)*config->TILE_WIDTH;
	hitBox.y = (config->LAB_SIZE/2)*config->TILE_HEIGHT;
	hitBox.w = PLAYER_WIDTH;
	hitBox.h = PLAYER_HEIGHT;

	printf("%d %d %d %d\n", hitBox.x, hitBox.y, hitBox.w, hitBox.h);

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	degrees = 0;
}


Player::~Player()
{
}

bool Player::touchesWall(SDL_Rect box)
{
	//Go through the tiles
	for (int i = 0; i < cfg->TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if (tiles->at(i)->getType() == cfg->TILE_WALL)
		{
			//If the collision box touches the wall tile
			if (Tile::checkCollision(box, tiles->at(i)->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

bool Player::touchesExit(SDL_Rect box)
{
	//Go through the tiles
	for (int i = 0; i < cfg->TOTAL_TILES; ++i)
	{
		//If the tile is an exit type tile
		if (tiles->at(i)->getType() == cfg->TILE_DOOR)
		{
			//If the collision box touches the wall tile
			if (Tile::checkCollision(box, tiles->at(i)->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

int Player::getStartxy()
{
	return hitBox.x;
}

SDL_Rect Player::getPlayerBox()
{
	return hitBox;
}

void Player::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= PLAYER_VEL; break;
		case SDLK_DOWN: mVelY += PLAYER_VEL; break;
		case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
		case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += PLAYER_VEL; break;
		case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
		case SDLK_LEFT: mVelX += PLAYER_VEL; break;
		case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
		}
	}
}

void Player::move()
{
	printf("%d %d %d %d %d\n", hitBox.x, hitBox.y, hitBox.w, hitBox.h, degrees);
	//Move the player left or right
	hitBox.x += mVelX;

	//Rotate player left or right
	if (mVelX < 0)
		rotatePosition(3);
	else if(mVelX > 0)
		rotatePosition(1);

	//If the player went too far to the left or right or touched a wall
	if ((hitBox.x < 0) || (hitBox.x + PLAYER_WIDTH > cfg->LEVEL_WIDTH || touchesWall(hitBox)))
	{
		//move back
		hitBox.x -= mVelX;
	}

	//Move the player up or down
	hitBox.y += mVelY;

	//Rotate player up or down
	if (mVelY < 0)
		rotatePosition(0);
	else if (mVelY > 0)
		rotatePosition(2);

	//If the player went too far up or down or touched a wall
	if ((hitBox.y < 0) || (hitBox.y + PLAYER_HEIGHT > cfg->LEVEL_HEIGHT) || touchesWall(hitBox))
	{
		//move back
		hitBox.y -= mVelY;
	}
}

void Player::setCamera(SDL_Rect& camera)
{
	//Center the camera over the dot
	camera.x = (hitBox.x + PLAYER_WIDTH / 2) - Utils::SCREEN_WIDTH / 2;
	camera.y = (hitBox.y + PLAYER_HEIGHT / 2) - Utils::SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > cfg->LEVEL_WIDTH - camera.w)
	{
		camera.x = cfg->LEVEL_WIDTH - camera.w;
	}
	if (camera.y > cfg->LEVEL_HEIGHT - camera.h)
	{
		camera.y = cfg->LEVEL_HEIGHT - camera.h;
	}
}

void Player::render(SDL_Rect& camera)
{
	//Show the Player
	cfg->playerTexture.render(cfg->renderer, hitBox.x - camera.x, hitBox.y - camera.y, NULL, degrees);
}

void Player::rotatePosition(int dir)
{
	switch (dir) {
	case 0:
		if (degrees != 0)
		{
			if (degrees > 0 && degrees <= 180)
				degrees -= ROTATION_SPEED;
			if (degrees > 180 && degrees <= 360)
			{
				degrees += ROTATION_SPEED;
				if (degrees == 360)
					degrees = 0;
			}
		}
		break;
	case 1:
		if (degrees != 90)
		{
			if (degrees > 90 && degrees <= 270)
				degrees -= ROTATION_SPEED;
			if (degrees > 270 || degrees <= 90)
			{
				degrees += ROTATION_SPEED;
				if (degrees == 360)
					degrees = 0;
			}
		}
		break;
	case 2:
		if (degrees != 180)
		{
			if (degrees >= 0 && degrees <= 180)
				degrees += ROTATION_SPEED;
			if (degrees > 180 && degrees <= 360)
			{
				degrees -= ROTATION_SPEED;
				if (degrees == 360)
					degrees = 0;
			}
		}
		break;
	case 3:
		if (degrees != 270)
		{
			if (degrees > 270 || degrees <= 90)
			{
				if (degrees == 0)
					degrees = 360;
				degrees -= ROTATION_SPEED;
			}
			if (degrees > 90 && degrees <= 270)
			{
				degrees += ROTATION_SPEED;
			}
		}
		break;
	}
}

#endif