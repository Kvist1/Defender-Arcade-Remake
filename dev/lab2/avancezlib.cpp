#include "SDL.h"
#include "avancezlib.h"
#include <stdlib.h>

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

	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}

	//Init SDL_ttf library
	if (font == NULL)
	{
		//TTF_Init
	}

	return true;
}

void AvancezLib::destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int AvancezLib::getElapsedTime()
{
	return SDL_GetTicks();
}

void AvancezLib::drawText(int x, int y, const char* msg)
{

}

bool AvancezLib::update()
{
	SDL_SetRenderDrawColor(renderer, 0, rand()%255, rand()%255, rand()%255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	return true;
}