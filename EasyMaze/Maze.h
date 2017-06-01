#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm> 
#include <random> 
#include <chrono>
#include "Tile.h"

class Maze
{
public:
	Maze(int size);
	~Maze();

	void show(); // Display structure of generated maze in stdout
	void generate(); 
	void refresh();
	int getSize(); // Return size of maze (N * N)

	bool isWall(int x, int y);
	bool isExit(int x, int y);

	std::vector<std::vector<Tile>> maze_map;
private:
	void setStart(int x, int y, bool state);
	void setWall(int x, int y, bool wall);
	void setExit(int x, int y, bool state);

	bool isStart(int x, int y);
	int getRand(int min, int max);
	std::vector<short> getRandDirections();
	std::pair<int, int> getStart();

	int placePath(std::pair<short, short> ptr);
	bool placeExit();
	bool placeStart();

	int _size;
};

#endif

