#pragma once
#include "LTimer.h"
#include "LTexture.h"

//Screen dimension constants
static constexpr int SCREEN_WIDTH = 640;
static constexpr int SCREEN_HEIGHT = 480;

static constexpr int number_of_rocks_being_drawn = 3;

static constexpr int player_lives = 30000;
static constexpr float player_direction = 40.0f;

//won't pollute global scope with all the names in Directions
namespace Direction {

	//unscoped enumerator for generating random directions of movemenet for rocks
	enum Direction {
		north,
		north_west,
		west,
		south_west,
		south,
		south_east,
		east,
		north_east
	};
}

struct Point {
	int x;
	int y;
};

struct Circle
{
	Point origin;
	int r;
};

//Circle/Circle collision detector
bool checkCollision(Circle& c1, Circle& c2);

//Calculates distance squared between two points
double distanceSquared(Point p1, Point p2);