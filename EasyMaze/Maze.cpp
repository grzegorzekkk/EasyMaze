#ifndef MAZE_CPP
#define MAZE_CPP

#include "Maze.h"

Maze::Maze(int size)
	: _size(size)
	, maze_map(size, std::vector<Tile>(size))
{

	if (_size < 7)
	{
		_size = 7;
		refresh();
	}
	if (_size % 2 == 0)
	{
		_size++;
		refresh();
	}
	Utils::setMazeSize(_size);
}

Maze::~Maze()
{
}

//print map (if succesfully generated...)
void Maze::show()
{
	for (int y = 0; y < _size; ++y)
	{
		for (int x = 0; x < _size; ++x)
		{
			if (isExit(x, y))
			{
				std::cout << '>';
				continue;
			}

			if (isStart(x, y))
			{
				std::cout << '0';
				continue;
			}

			if (isWall(x, y))
				std::cout << '#';
			else
				std::cout << '.';

		}
		std::cout << std::endl;
	}
}

void Maze::generate()
{
	std::pair<int, int> ptr = getStart();
	int dir;

	placeStart();
	dir=placePath(ptr);
	placeExit();
}

void Maze::refresh()
{
	maze_map.clear();
	maze_map.resize(_size);
	for (int j = 0; j < _size; ++j)
	{
		maze_map[j].resize(_size);
	}
}

int Maze::getSize()
{
	return _size;
}

void Maze::setStart(int x, int y, bool state)
{
	maze_map[y][x].start = state;
}

void Maze::setWall(int x, int y, bool wall)
{
	maze_map[y][x].wall = wall;
}

void Maze::setExit(int x, int y, bool state)
{
	maze_map[y][x].exit = state;
}

bool Maze::isStart(int x, int y)
{
	return maze_map[y][x].start;
}

bool Maze::isWall(int x, int y)
{
	return maze_map[y][x].wall;
}

bool Maze::isExit(int x, int y)
{
	return maze_map[y][x].exit;
}

int Maze::getRand(int min, int max)
{
	return min + (int)((double)rand() / (RAND_MAX + 1) * (max - min + 1));
}

std::vector<short> Maze::getRandDirections()
{
	std::vector<short> dirs{ 0,1,2,3 };

	random_shuffle(dirs.begin(), dirs.end());
	return dirs;
}

std::pair<int, int> Maze::getStart()
{
	return std::pair<short, short>(floor(_size/2.0), floor(_size/2.0));
}

int Maze::placePath(std::pair<short, short> ptr)
{
	std::vector<short> dirs = getRandDirections();

	for (int i = 0; i < dirs.size(); i++)
	{
		switch (dirs[i])
		{
		case 0:
			if (ptr.second - 2 < 1)
				continue;
			if (isWall(ptr.first, ptr.second - 2))
			{
				setWall(ptr.first, ptr.second - 1, false);
				setWall(ptr.first, ptr.second - 2, false);
				ptr.second -= 2;
				placePath(ptr);
			}
			break;
		case 1:
			if (ptr.first + 2 > _size - 1)
				continue;
			if (isWall(ptr.first + 2, ptr.second))
			{
				setWall(ptr.first + 1, ptr.second, false);
				setWall(ptr.first + 2, ptr.second, false);
				ptr.first += 2;
				placePath(ptr);
			}
			break;
		case 2:
			if (ptr.second + 2 > _size - 1)
				continue;
			if (isWall(ptr.first, ptr.second+2))
			{
				setWall(ptr.first, ptr.second+1, false);
				setWall(ptr.first, ptr.second+2, false);
				ptr.second += 2;
				placePath(ptr);
			}
			break;
		case 3:
			if (ptr.first - 2 < 1)
				continue;
			if (isWall(ptr.first - 2, ptr.second))
			{
				setWall(ptr.first - 1, ptr.second, false);
				setWall(ptr.first - 2, ptr.second, false);
				ptr.first -= 2;
				placePath(ptr);
			}
			break;
		}
	}

	return 0;
}

bool Maze::placeStart()
{
	int xy = (int)floor(_size / 2.0);

	setStart(xy, xy, true);
	return true;
}

bool Maze::placeExit()
{
	short dir = getRand(0, 3);

	switch (dir) 
	{
	case 0:
	{
		short x = getRand(1, _size - 1);
		short y = 0;
		if (isWall(x, y + 1))
			placeExit();
		else
		{
			setExit(x, y, true);
			setWall(x, y, false);
		}
	}
		break;
	case 1:
	{
		short x = _size - 1;
		short y = getRand(1, _size - 1);
		if (isWall(x - 1, y))
			placeExit();
		else
		{
			setExit(x, y, true);
			setWall(x, y, false);
		}
	}
		break;
	case 2:
	{
		short x = getRand(1, _size - 1);
		short y = _size - 1;
		if (isWall(x, y - 1))
			placeExit();
		else
		{
			setExit(x, y, true);
			setWall(x, y, false);
		}
	}
	break;
	case 3:
	{
		short x = 0;
		short y = getRand(1, _size - 1);
		if (isWall(x + 1, y))
			placeExit();
		else
		{
			setExit(x, y, true);
			setWall(x, y, false);
		}
	}
	break;
	}

	return true;
}

#endif

