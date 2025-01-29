#pragma once
#include <SDL.h>
#include "include/glad/glad.h"
#include "include/KHR/khrplatform.h"
#include <GLFW/glfw3.h>
#include "Constants.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Rocks.h"
#include <vector>
#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class Player
{
public:
	//The dimensions of the player
	static constexpr int player_WIDTH = 25;
	static constexpr int player_HEIGHT = 20;

	static constexpr int add_by = 15;

	int m_player_lives;

	//Maximum axis velocity of the player
	static const int TRIANGLE_VEL = 10;

	// Variables to hold the mouse position
	int mouse_x_position, mouse_y_position;

	glm::vec2 playerDirection;

	//Player as triangle
	SDL_Vertex triangleVertex[3] =
	{
	 {
	  { (float)mPosX + add_by, (float)mPosY }, /* first point location */
	  { 255, 0, 0, 0xFF }, /* first color */
	  { 0.f, 0.f }
	 },
	 {
	  { (float)(mPosX - (player_WIDTH / 2) + add_by), (float)(mPosY + player_HEIGHT) }, /* second point location */
	  { 0,255,0, 0xFF }, /* second color */
	  { 0.f, 0.f }
	 },
	 {
	  { (float)(mPosX + (player_WIDTH / 2) + add_by), (float)(mPosY + player_HEIGHT) }, /* third point location */
	  { 0,0,255, 0xFF }, /* third color */
	  { 0.f, 0.f }
	 }
	};

public:

	Player(int x, int y, int player_lives, float direction);

	//Get the current rotation angle in degrees
	double get_rotation() const;

	//Update rotation based on mouse position
	void update_rotation(int mouse_x, int mouse_y);

	//Takes key presses and adjusts the player's velocity
	void handleEvent(SDL_Event& e);

	//Moves the rocks and checks collision
	int move(float deltaTime, std::vector<Rocks>& rocks);

	//Shows the player on the screen
	void render(LTexture& gDotTexture);

	//Gets collision circle
	const Circle& getCollider();

	void updatePlayerDirection(float angle);

	void handleMouseRotation(int mouse_x_position, int mouse_y_position);

	SDL_Vertex* getVertices();

	void movePositionXandYForPlayer();


	int get_x() const {
		return mPosX;
	}

	int get_y() const {
		return mPosY;
	}

	struct Point {
		int x;
		int y;
	};

	Point get_m_x_and_y() const {
		return { mPosX, mPosY };
	}

	int get_width() const {
		return player_WIDTH;
	}

	int get_height() const {
		return player_HEIGHT;
	}

	std::pair<int, int> get_tip_triangle_position() const {
		return { triangleVertex[0].position.x, triangleVertex[0].position.y };
	}

private:
	//Player's x and y positions
	int mPosX, mPosY;

	//The velocity of the player
	int mVelX, mVelY;

	//Current rotation angle in degrees
	double m_rotation;

	float m_direction;

	//Using a circle collision for now
	Circle mCollider;

	//Moves the collision circle relative to the triangle's offset
	void shiftColliders();

	// Apply rotation to vertices
	void applyRotation(float angle);  

	glm::vec2 calculateCenter();

	float getCurrentPlayerAngle();

	glm::mat4 model = glm::mat4(1.0f);

	int last_mouse_x_position = -1;
	int last_mouse_y_position = -1;
};