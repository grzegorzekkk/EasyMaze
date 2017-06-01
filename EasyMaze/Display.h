#ifndef DISPLAY_H
#define DISPLAY_H

#include"Texture.h"
#include"Maze.h"
#include"Utils.h"
#include"Display.h"
#include"Player.h"
#include<SDL.h>
#include<SDL_image.h>
#include<stdio.h>
#include<iostream>
#include<string>
#include<sstream>

class Display
{
public:
	Display(int width, int height, char *window_name, Utils *config);
	~Display();
	void close(); // Free resources and close the window
	bool setTiles(Maze *mazeptr); // Prepare tiles based on generated maze map

	bool loadMaze(Maze *mazeptr); // Generate maze texture
	bool showMenu(SDL_Event &e); // Display main menu
	bool showCredits(SDL_Event &e); // Display author info
	bool showSummary(SDL_Event &e, double time); // Display summary after escaping the maze
	bool showRules(SDL_Event &e); // Display rules of the game
	int sizePrompt(SDL_Event &e); // Get the size of maze from player

	std::vector<Tile*> tiles;
private:
	bool loadMedia();

	void renderBackground();
	void showText(std::string text, int width, int height, SDL_Color color = { 255, 255, 255, 0xFF });

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	Utils *cfg;
};

#endif

