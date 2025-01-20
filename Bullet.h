#pragma once
#include "Player.h"

class Bullet {
public:
	Bullet(double m_x, double m_y);
	void handleEvent_fire(SDL_Event& e);

	void fire(const Player& player);
	void update();

	void render_and_shoot_bullet(LTexture& bullet_image, SDL_Rect& gbullet, const Player& player);
	void increase_speed();
	void increase_bullets();	

private:
	double m_x;
	double m_y;

	//velocity
	double m_dx;
	double m_dy; 

	//bullet spped
	double m_speed;

	//whether bullet is currently in flight
	bool m_is_active;  

	bool m_mouse_fire_key_pressed = false;
	
};