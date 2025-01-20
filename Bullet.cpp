#include "Bullet.h"
#include "Player.h"
#include <iostream>
  
Bullet::Bullet(double x, double y) : 
m_x(x), m_y(0.0), m_dy(0.0), m_speed(10.0), m_is_active(false), m_mouse_fire_key_pressed(false)
{
}
void Bullet::handleEvent_fire(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {

        if (!m_is_active) {
            m_mouse_fire_key_pressed = true;
        }
        //std::cout << m_mouse_fire_key_pressed;
    }
}

void Bullet::fire(const Player& player) {
    if (!m_is_active && m_mouse_fire_key_pressed) {
        m_is_active = true;
        // Reset the flag after firing
        m_mouse_fire_key_pressed = false;  

        // Get player's rotation angle in degrees
        double angle = player.get_rotation();

        // Convert angle to radians
        double radians = angle * M_PI / 180.0;

        // Calculate velocity 
        m_dx = m_speed * cos(radians);
        m_dy = m_speed * sin(radians);

        //Set initial position to player's tip
        m_x = player.get_tip_triangle_position().first;
        m_y = player.get_tip_triangle_position().second;

        //std::cout << m_x << "\n";
        //std::cout << m_y << "\n";
    }
}

void Bullet::update() {
    if (m_is_active) {
        // Update position based on velocity
        m_x += m_dx;
        m_y += m_dy;

        // Deactivate bullet if it goes off screen
        if (m_x < 0 || m_x > SCREEN_WIDTH || m_y < 0 || m_y > SCREEN_HEIGHT) {
            m_is_active = false;
        }
    }
}

void Bullet::render_and_shoot_bullet(LTexture& bullet_image, SDL_Rect& gbullet, const Player& player)
{
    fire(player);

    if (m_is_active)
    {
        const int subtract_from_y_so_bullet_above_player = 20;
        const int subtract_from_x_so_bullet_in_middle = 6;

        // Update bullet position
        update();

        //render bullet at current position
        bullet_image.render(m_x - subtract_from_x_so_bullet_in_middle,
            m_y - subtract_from_y_so_bullet_above_player,
            &gbullet);

        //making sure the bullet gets destroyed after out of screen
        //std::cout << m_x << "\n";
        //std::cout << m_y << "\n";
    }
}

void Bullet::increase_speed()
{

}

void Bullet::increase_bullets()
{

}

