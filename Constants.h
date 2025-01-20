#pragma once
//constants in this file
#include "LTimer.h"
#include "LTexture.h"

//Screen dimension constants
static constexpr int SCREEN_WIDTH = 640;
static constexpr int SCREEN_HEIGHT = 480;

static constexpr int number_of_rocks_being_drawn = 3;
// If not static ODR violation will occur

//The spriteclips were not working when they were in constants.h and kind of work when I moved them to the file with main function.
//static SDL_Rect gSpriteClips[number_of_rocks_being_drawn];
//static SDL_Rect 

//LTexture gDotTexture;
//LTexture grock_image;
//std::vector <SDL_Rect> gSpriteClips(number_of_rocks_being_drawn);

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

struct Circle
{
	int x, y;
	int r;
};

//Circle/Circle collision detector
bool checkCollision(Circle& a, Circle& b);

//Calculates distance squared between two points
double distanceSquared(int x1, int y1, int x2, int y2);