#include <random>
#include "Rocks.h"
#include "Constants.h"

Rocks::Rocks()
{	
	// for seeding
	std::random_device dev; 
	std::default_random_engine gen{ dev() };
	std::uniform_int_distribution<int> first_dis{ 0, SCREEN_WIDTH - 320};
	std::uniform_int_distribution<int> second_dis{ 0, SCREEN_HEIGHT - 360};
	// The - 360 is so image doesn't get drawn too far away from the screen
	m_x = first_dis(gen);
	m_y = second_dis(gen);
	m_broken_or_not = false;
	mCollider.r = m_width / 2;

	//Move collider relative to the circle
	shiftColliders();
}

Rocks::Rocks(int x, int y, bool broken) : m_x(x), m_y(y), m_broken_or_not(broken)
{
	mCollider.r = m_width / 2;

	//Move collider relative to the circle
	shiftColliders();
}


//making the rocks move in a specific direction according to where they spawn at the start.

void Rocks::choose_random_direction_for_rocks()
{
	std::random_device dev; // for seeding
	std::default_random_engine gen{ dev() };
	std::uniform_int_distribution<int> random_direction{ 1, 8 };

	const int direction = random_direction(gen);

	switch (direction) {
	case 1:
		rock_direction = Direction::north;
		break;
	case 2:
		rock_direction = Direction::north_west;
		break;
	case 3:
		rock_direction = Direction::west;
		break;
	case 4:
		rock_direction = Direction::south_west;
		break;
	case 5:
		rock_direction = Direction::south;
		break;
	case 6:
		rock_direction = Direction::south_east;
		break;
	case 7:
		rock_direction = Direction::east;
		break;
	case 8:
		rock_direction = Direction::north_east;
		break;
	}
}

void Rocks::make_rocks_move()
{
	switch (rock_direction) {

	case Direction::north:
		m_y += m_velocity_y;
		break;
	case Direction::north_west:
		m_x -= m_velocity_x;
		m_y += m_velocity_y;
		break;
	case Direction::west:
		m_x -= m_velocity_x;
		break;
	case Direction::south_west:
		m_x -= m_velocity_x;
		m_y -= m_velocity_y;
		break;
	case Direction::south:
		m_y -= m_velocity_y;
		break;
	case Direction::south_east:
		m_x += m_velocity_x;
		m_y -= m_velocity_y;
		break;
	case Direction::east:
		m_x += m_velocity_x;
		break;
	case Direction::north_east:
		m_x += m_velocity_x;
		m_y += m_velocity_y;
		break;
	}
}

void Rocks::make_rocks_spawn_opposite_side_when_out_of_screen(int screen_width, int screen_height)
{

	if (m_x > screen_width) {
		m_x = 0;
	}
	else if (m_x < 0) {
		m_x = screen_width;
	}
	if (m_y >= screen_height) {
		m_y = 0;
	}
	else if (m_y <= 0) {
		m_y = screen_height;
	}

}

//Shows the rocks on the screen
void Rocks::render(LTexture& rocks_image, SDL_Rect gSpriteClips[number_of_rocks_being_drawn]) {

	//Show the rocks
	for (int i = 0; i < number_of_rocks_being_drawn; i++) {
		rocks_image.render(m_x, m_y, &gSpriteClips[i]);
	}
}

Circle& Rocks::getCollider()
{
	return mCollider;
}

//Moves the collision circle relative to the Rocks's offset
void Rocks::shiftColliders() {
	mCollider.x = m_x;
	mCollider.y = m_y;
}

