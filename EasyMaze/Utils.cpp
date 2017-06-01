#ifndef UTILS_CPP
#define UTILS_CPP

#include "Utils.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

int Utils::LAB_SIZE = 0;

//Screen dimension
int Utils::SCREEN_WIDTH = 0;
int Utils::SCREEN_HEIGHT = 0;

void Utils::setMazeSize(int size)
{
	LAB_SIZE = size;
}

#endif
