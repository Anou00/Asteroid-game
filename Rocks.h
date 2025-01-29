#pragma once
#include "LTexture.h"
#include "Constants.h"

class Rocks {
	public:
		Rocks();
		Rocks(int x, int y, bool broken);
		
		//enum variable in namespace
		Direction::Direction rock_direction;
	
		//void spawn_multiple_rocks();

		void choose_random_direction_for_rocks();
		void make_rocks_move();
		void make_rocks_spawn_opposite_side_when_out_of_screen(int screen_width, int screen_height);
		

		//bool check_for_collision_between_player_rock(int& lives, bool& game_over, Player& player);

		//Shows the rocks on the screen
		void render(LTexture& rocks_image, SDL_Rect gSpriteClips[number_of_rocks_being_drawn]);

		//Gets collision circle
		Circle& getCollider();

		int get_m_length() const
		{
			return m_length;
		}

		int get_m_width() const
		{
			return m_width;
		}

		int get_m_x() const
		{
			return m_x;
		}

		int get_m_y() const
		{
			return m_y;
		}

		struct Point {
			int x;
			int y;
		};

		Point get_m_x_and_y() const {
			return { m_x, m_y };
		}

		int get_m_velocity_x() const
		{
			return m_velocity_x;
		}

		int get_m_velocity_y() const
		{
			return m_velocity_y;
		}

	private:
		static const int m_length = 200, m_width = 200;
		static const int m_velocity_x = 1, m_velocity_y = 1;
		int m_x, m_y;
		bool m_broken_or_not;

		//Rock's collision circle
		Circle mCollider;

		//Moves the collision circle relative to the Rocks's offset
		void shiftColliders();
};