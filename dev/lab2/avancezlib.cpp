#include "SDL.h"
#include "avancezlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

bool AvancezLib::init(int width, int height) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL failed the initialization: %s\n", SDL_GetError());
	}

	//Create window
	window = SDL_CreateWindow("Lab One", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}

	//Initialize SDL_ttf 
	if (TTF_Init() == -1)
	{
		SDL_Log("TTF could not be created! SDL Error: %s\n", SDL_GetError());
	}

	return true;
}

void AvancezLib::destroy()
{
	// Free global font
	TTF_CloseFont(font);
	font = NULL;

	// Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	// Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

int AvancezLib::getElapsedTime()
{
	return SDL_GetTicks();
}

void AvancezLib::calculateFPS(int maxFps, int frameTicks)
{
	float delay = 1000.f / (float)maxFps - (float)frameTicks;
	// / 10 * 10 removes the decimals
	fps = 1000.f / delay / 10 * 10;
}

void AvancezLib::drawText(int x, int y, const char* msg)
{
	font = TTF_OpenFont("space_invaders.ttf", 24);

	SDL_Color fontColorWhite = { 255, 255, 255 };

	// as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, msg, fontColorWhite);

	// now you can convert it into a texture
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_FreeSurface(surfaceMessage);

	SDL_Rect message_rect; //create a rect
	message_rect.x = x;  //controls the rect's x coordinate 
	message_rect.y = y; // controls the rect's y coordinte
	message_rect.w = 50; // controls the width of the rect
	message_rect.h = 20; // controls the height of the rect

	// Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, 
	// that way it would be very simple to understance

	// Now since it's a texture, you have to put RenderCopy in your game loop area, 
	// the area where the whole code executes

	// you put the renderer's name first, the Message, the crop size
	// (you can ignore this if you don't want to dabble with cropping), 
	// and the rect which is the size and coordinate of your texture
	SDL_RenderCopy(renderer, message, NULL, &message_rect); 
	SDL_DestroyTexture(message);
}

bool AvancezLib::update()
{
	// Draw random color
	SDL_SetRenderDrawColor(renderer, 0, rand()%255, rand()%255, rand()%255);
	SDL_RenderClear(renderer);

	// Draw a fps counter
	std::string fps_text = "FPS: " + std::to_string(fps);
	drawText(10, 10, fps_text.c_str());

	SDL_RenderPresent(renderer);

	return true;
}