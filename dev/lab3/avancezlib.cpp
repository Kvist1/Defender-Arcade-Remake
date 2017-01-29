#include "SDL.h"
#include "avancezlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

bool AvancezLib::init(int width, int height) 
{
	SDL_Log("Initializing the system...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Initialize SDL_ttf 
	TTF_Init();
	font = TTF_OpenFont("data/space_invaders.ttf", 12); //this opens a font style and sets a size
	if (font == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// init key statuses
	key.fire = false;
	key.left = false;
	key.right = false;

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Clear screen
	SDL_RenderClear(renderer);

	SDL_Log("System up and running...\n");
	return true;
}

// Destroys the avancez library instance
void AvancezLib::destroy()
{
	SDL_Log("Shutting down the system\n");

	// Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	// Free global font
	TTF_CloseFont(font);
	font = NULL;

	// Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}


//bool AvancezLib::update()
//{
//	// Draw random color
//	SDL_SetRenderDrawColor(renderer, 0, rand()%255, rand()%255, rand()%255);
//	SDL_RenderClear(renderer);
//
//	// Draw a fps counter
//	std::string fps_text = "FPS: " + std::to_string(fps);
//	drawText(10, 10, fps_text.c_str());
//
//	SDL_RenderPresent(renderer);
//
//	return true;
//}


// Main game loop
bool AvancezLib::update()
{
	bool go_on = true; 
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = true;
				break;
			case SDLK_RIGHT:
				key.right = true;
				break;
			case SDLK_SPACE:
				key.fire = true;
				break;
			case SDLK_ESCAPE:
			case SDLK_q:
				go_on = false;
				break;
			}
		}
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				key.left = false;

				break;
			case SDLK_RIGHT:
				key.right = false;
				break;
			case SDLK_SPACE:
				key.fire = false;
				break;
			}
		}

		if (event.type == SDL_QUIT) go_on = false;
	}

	//Update screen
	SDL_RenderPresent(renderer);
	
	//SDL_SetRenderDrawColor(renderer, 0, rand()%255, rand()%255, rand()%255);

	//Clear screen
	SDL_RenderClear(renderer);

	return go_on;
}


Sprite * AvancezLib::createSprite(const char * path)
{
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (surf == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);

	Sprite * sprite = new Sprite(renderer, texture);

	return sprite;
}


//void AvancezLib::calculateFPS(int maxFps, int frameTicks)
//{
//	float delay = 1000.f / (float)maxFps - (float)frameTicks;
//	// / 10 * 10 removes the decimals
//	fps = 1000.f / delay / 10 * 10;
//}

void AvancezLib::drawText(int x, int y, const char * msg)
{
	// this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Color black = { 0, 0, 0 };

	// as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Surface* surf = TTF_RenderText_Solid(font, msg, black);

	//now you can convert it into a texture
	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf);

	int w = 0;
	int h = 0;
	SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
	SDL_Rect dst_rect = { x, y, w, h };

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surf);
}

int AvancezLib::getElapsedTime()
{
	return SDL_GetTicks();
}

void AvancezLib::setDelta(int delta)
{
	delta_ = delta;
}

int AvancezLib::getDelta()
{
	return delta_;
}

void AvancezLib::getKeyStatus(KeyStatus& keys)
{
	keys.fire = key.fire;
	keys.left = key.left;
	keys.right = key.right;
}


Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture)
{
	this->renderer = renderer;
	this->texture = texture;
}


void Sprite::draw(int x, int y)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


void Sprite::destroy()
{
	SDL_DestroyTexture(texture);
}