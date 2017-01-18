#include "SDL.h"

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL failed the initialization: %s\n", SDL_GetError());
	}

	//Create window
	SDL_Window * window = SDL_CreateWindow("Lab One", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	//Create renderer for window
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0x55, 0xFF, 0xFF, 0xFF);

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
	SDL_Delay(10000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}