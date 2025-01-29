#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include "Constants.h"
#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <random>
#include <memory>
#include "Player.h"
#include "Rocks.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Game.h"


SDL_Rect gSpriteClips[number_of_rocks_being_drawn];
SDL_Rect gbullet = { 0, 0, 32, 32 };

Game game;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Scene textures
LTexture gDotTexture;
LTexture grock_image;
LTexture gbullet_image;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == nullptr)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	// Load asteroid image I got online
	if (!grock_image.loadFromFile("Assets/360x360 rock_image.jpg"))
	{
		printf("Failed to load image of asteroid!\n");
		success = false;
	}
	else {
	
		for (int i = 0; i < number_of_rocks_being_drawn; i++) {

			gSpriteClips[i].x = game.state.rocks[i].get_m_x();
			gSpriteClips[i].y = game.state.rocks[i].get_m_y();
			gSpriteClips[i].w = 270;
			gSpriteClips[i].h = 270;
		}
			
	}

	if (!gbullet_image.loadFromFile("Assets/asteroid game bullets.png"))
	{
		printf("Failed to load asteroid bullets texture!\n");
		success = false;
	}
	else {
		//Crop only one bullet that the asteroid will shoot.
		std::pair <int, int> x_and_y = game.state.player.get_tip_triangle_position();
		int bullet_x = x_and_y.first;
		int bullet_y = x_and_y.second;
		const int bullet_x_addition = 30;
		const int bullet_y_addition = 94;
		gbullet.x = bullet_x + bullet_x_addition;
		gbullet.y = bullet_y + bullet_y_addition;
		gbullet.w = 10;
		gbullet.h = 20;
	}

	return success;
}


void close()
{
	//Free loaded images
	gDotTexture.free();
	grock_image.free();
	gbullet_image.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

LTexture::LTexture()
{
	//Initialize
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != nullptr;
}


#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != nullptr)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == nullptr)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != nullptr;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

#undef main
int main(int argc, char* argv[]) {
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			// Random directions for rocks before while loop, so rocks directions are set once.
			game.random_direction_for_rocks();

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					game.state.player.handleEvent(e);
					game.handleEvent_fire(e);

				}  

				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);

				game.update_rotation_of_player(mouseX, mouseY);

				// Handle player rotation based on mouse position
				game.state.player.handleMouseRotation(mouseX, mouseY);


				game.move_rocks_and_spawn_out_screen();
				int lives = game.move_player();

				game.movePositionXandYForPlayer();

				//If player loses all his lives game ends
				if (lives <= 0) quit = true;

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//render the rock images here so they're drawn above the player or dot when they collide.
				game.render_rocks(grock_image, gSpriteClips);

				//Render objects
				if (SDL_RenderGeometry(gRenderer, nullptr, game.state.player.triangleVertex, 3, nullptr, 0) < 0) { SDL_Log("%s\n", SDL_GetError()); }

				// Render the player 
				SDL_Vertex* vertices = game.state.player.getVertices();


				//bullets
				game.render_and_shoot_bullet(gbullet_image, gbullet);

				//Update screen
				SDL_RenderPresent(gRenderer);

			}
		}
	}
	close();
	return 0;
}


