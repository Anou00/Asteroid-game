#include "Game.h"

Game::Game()
{
}

void Game::hande_input(SDL_Event& e)
{
	state.player.handleEvent(e);
}

void Game::random_direction_for_rocks()
{
	for (int i = 0; i < state.rocks.size(); i++) {
		state.rocks[i].choose_random_direction_for_rocks();
	}
}

void Game::move_rocks_and_spawn_out_screen()
{
	for (int i = 0; i < state.rocks.size(); i++) {
		state.rocks[i].make_rocks_move();
		state.rocks[i].make_rocks_spawn_opposite_side_when_out_of_screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

void Game::render_rocks(LTexture& rocks_image, SDL_Rect gSpriteClips[number_of_rocks_being_drawn])
{
	for (int i = 0; i < state.rocks.size(); i++) {
		state.rocks[i].render(rocks_image, gSpriteClips);
	}
}

void Game::update_rotation_of_player(int mouse_x, int mouse_y)
{
	state.player.update_rotation(mouse_x, mouse_y);
}

int Game::move_player()
{
	float deltaTime = 0.1f;
	int lives = state.player.move(deltaTime, state.rocks);
	return lives;
}

void Game::movePositionXandYForPlayer()
{
	state.player.movePositionXandYForPlayer();
}

void Game::handleEvent_fire(SDL_Event& e)
{
	state.bullet.handleEvent_fire(e);
}

void Game::render_and_shoot_bullet(LTexture& bullet_image, SDL_Rect& gbullet)
{
	state.bullet.render_and_shoot_bullet(bullet_image, gbullet, state.player);
}
