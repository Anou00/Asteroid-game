#pragma once
#include "Constants.h"
#include "Player.h"
#include "Rocks.h"
#include "Bullet.h"

struct GameState {

	Player player {Player::player_WIDTH / 2, Player::player_HEIGHT / 2, 30000, 40};
	std::vector<Rocks> rocks {number_of_rocks_being_drawn};
	Bullet bullet{(double)(player.get_tip_triangle_position().first), (double)(player.get_tip_triangle_position().second)};
};

class Game {
public:
	GameState state;

	public:
		Game();
		void hande_input(SDL_Event& e);

		//rocks
		void random_direction_for_rocks();
		void move_rocks_and_spawn_out_screen();
		void render_rocks(LTexture& rocks_image, SDL_Rect gSpriteClips[number_of_rocks_being_drawn]);
		
		//player
		double get_rotation_of_player() const;
		void update_rotation_of_player(int mouse_x, int mouse_y);
		int move_player();
		void movePositionXandYForPlayer();


		//bullets
		void handleEvent_fire(SDL_Event& e);
		void render_and_shoot_bullet(LTexture& bullet_image, SDL_Rect& gbullet); 
		
};
	
	