#include "Player.h"
#include <iostream>

Player::Player(int x, int y, int player_lives, float direction) : mPosX(x), mPosY(y),
m_player_lives(player_lives), m_direction(direction)
{

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Set collision circle size
	mCollider.r = player_WIDTH / 2;

	m_rotation = 0;

	//Move collider relative to the circle
	shiftColliders();

	playerDirection = glm::vec2(0.0f, 1.0f);

}

// Get the current rotation angle in degrees
double Player::get_rotation() const {
	return m_rotation;
}

// Update rotation based on mouse position
void Player::update_rotation(int mouse_x, int mouse_y) {
	// Calculate the difference between mouse and player position
	double dx = mouse_x - mPosX;
	double dy = mouse_y - mPosY;

	// Calculate angle in radians
	double angle_radians = atan2(dy, dx);

	// Convert to degrees
	m_rotation = angle_radians * 180.0 / M_PI;
}

void Player::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= (int)TRIANGLE_VEL; break;
		case SDLK_DOWN: mVelY += (int)TRIANGLE_VEL; break;
		case SDLK_LEFT: mVelX -= (int)TRIANGLE_VEL; break;
		case SDLK_RIGHT: mVelX += (int)TRIANGLE_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += (int)TRIANGLE_VEL; break;
		case SDLK_DOWN: mVelY -= (int)TRIANGLE_VEL; break;
		case SDLK_LEFT: mVelX += (int)TRIANGLE_VEL; break;
		case SDLK_RIGHT: mVelX -= (int)TRIANGLE_VEL; break;
		}
	}

	if (e.type == SDL_MOUSEMOTION) {
		int mouse_x = e.motion.x;
		int mouse_y = e.motion.y;
		update_rotation(mouse_x, mouse_y);
	}
}

int Player::move(float deltaTime, std::vector<Rocks>& rocks)
{
	// Move the player left or right
	triangleVertex[0].position.x += mVelX * deltaTime;
	triangleVertex[1].position.x += mVelX * deltaTime;
	triangleVertex[2].position.x += mVelX * deltaTime;

	// Shift colliders to update position
	shiftColliders();

	for (int i = 0; i < number_of_rocks_being_drawn; i++) {
		if ((triangleVertex[0].position.x - player_WIDTH / 2 < 0) || (triangleVertex[0].position.x + player_WIDTH / 2 > SCREEN_WIDTH))
		{
			// Move back if out of bounds
			triangleVertex[0].position.x -= mVelX * deltaTime;
			triangleVertex[1].position.x -= mVelX * deltaTime;
			triangleVertex[2].position.x -= mVelX * deltaTime;
			shiftColliders();
		}

		// Move the player up or down
		triangleVertex[0].position.y += mVelY * deltaTime;
		triangleVertex[1].position.y += mVelY * deltaTime;
		triangleVertex[2].position.y += mVelY * deltaTime;

		shiftColliders();

		// Collision detection with rocks
		if ((triangleVertex[0].position.y < 0) || (triangleVertex[0].position.y + player_HEIGHT / 2 + 20 > SCREEN_HEIGHT))
		{
			// Prevent player from moving out of bounds
			triangleVertex[0].position.y -= mVelY * deltaTime;
			triangleVertex[1].position.y -= mVelY * deltaTime;
			triangleVertex[2].position.y -= mVelY * deltaTime;
			shiftColliders();
		}

		if (checkCollision(mCollider, rocks[i].getCollider()))
		{
			m_player_lives--; // Decrease lives on collision
			shiftColliders();
		}
	}

	// Ensure player position is updated after movement and rotation
	movePositionXandYForPlayer();
	return m_player_lives;
}


void Player::render(LTexture& gDotTexture)
{
	//Show the triangle player
	gDotTexture.render(mPosX, mPosY);
}

const Circle& Player::getCollider()
{
	return mCollider;
}

void Player::movePositionXandYForPlayer() {

	// Shift mPosX and mPosY 
	mPosX = triangleVertex[0].position.x;
	mPosY = triangleVertex[0].position.y;
}

void Player::shiftColliders()
{
	mCollider.origin.x = triangleVertex[0].position.x;
	mCollider.origin.y = triangleVertex[0].position.y;
}

SDL_Vertex* Player::getVertices() {
	// Return the triangle vertices
	return triangleVertex;
}

// Method to update the player's facing direction based on an angle in radians
void Player::updatePlayerDirection(float angle) {
	// Convert angle to unit vector
	playerDirection = glm::vec2(cos(angle), sin(angle));

	//std::cout << playerDirection.x << "\n";
	//std::cout << playerDirection.y << "\n";
}

void Player::handleMouseRotation(int mouse_x, int mouse_y)
{
	// Rotation speed and delta time
	const float rotationSpeed = 0.1f;
	const float deltaTime = 0.05f;

	// Only update rotation if the mouse has moved
	if (mouse_x != last_mouse_x_position || mouse_y != last_mouse_y_position)
	{
		// Calculate the direction from the player to the mouse
		glm::vec2 direction = glm::normalize(glm::vec2(mPosX - mouse_x, mPosY - mouse_y));

		// Calculate target angle (from mouse position)
		float targetAngle = atan2(direction.y, direction.x);

		// Get current angle of the player
		float currentAngle = getCurrentPlayerAngle();

		// Calculate the shortest rotation angle
		float angleDifference = currentAngle - targetAngle;

		// Normalize the angle difference to [-PI, PI]
		if (angleDifference > glm::pi<float>())
		{
			angleDifference -= 2.0f * glm::pi<float>();
		}
		else if (angleDifference < -glm::pi<float>())
		{
			angleDifference += 2.0f * glm::pi<float>();
		}

		// Apply rotation speed limiting and reverse the direction
		float rotation = -glm::clamp(angleDifference, -rotationSpeed, rotationSpeed); 

		// Update the player's direction
		float newAngle = currentAngle + rotation;
		updatePlayerDirection(newAngle);

		// Apply the rotation to the triangle vertices
		applyRotation(rotation);

		// Update last mouse position
		last_mouse_x_position = mouse_x;
		last_mouse_y_position = mouse_y;
	}
}

void Player::applyRotation(float angle)
{
	// Create the rotation matrix
	glm::mat2 rotationMatrix = glm::mat2(
		glm::cos(angle), glm::sin(angle),
		-glm::sin(angle), glm::cos(angle)
	);

	// Get the center of the triangle
	glm::vec2 center = calculateCenter();

	// Rotate each vertex around the center
	for (int i = 0; i < 3; ++i)
	{
		glm::vec2 vertexPos = glm::vec2(triangleVertex[i].position.x, triangleVertex[i].position.y);

		// Translate to origin
		vertexPos -= center;

		// Apply rotation
		vertexPos = rotationMatrix * vertexPos;

		// Translate back
		vertexPos += center;

		// Update vertex position
		triangleVertex[i].position.x = vertexPos.x;
		triangleVertex[i].position.y = vertexPos.y;
	}
}


glm::vec2 Player::calculateCenter()
{
	// Center of the triangle player 
	glm::vec2 center = glm::vec2(
		(triangleVertex[0].position.x + triangleVertex[1].position.x + triangleVertex[2].position.x) / 3,
		(triangleVertex[0].position.y + triangleVertex[1].position.y + triangleVertex[2].position.y) / 3
	);
	return center;
}

// Method to get the player's current facing angle in radians
float Player::getCurrentPlayerAngle() {
	// Returns angle in radians
	return glm::atan(playerDirection.y, playerDirection.x);  
}


