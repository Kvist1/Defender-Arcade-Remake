#include "SDL.h"
#include "avancezlib.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	AvancezLib system;
	SDL_Event event;
	bool gameRunning = true;
	int startTime;
	int MS_PER_FRAME = 16;

	/* Initialise SDL and video modes and all that */
	system.init(900, 640);


	/* Main game loop */
	while (gameRunning) 
	{
		startTime = system.getElapsedTime();

		/* Check for events */
		while (SDL_PollEvent(&event)) 
		{
			/* Look for a keypress */
			if (event.type == SDL_KEYDOWN) 
			{
				/* Check the SDLKey values and move change the coords */
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					//alien_x -= 1;
					break;
				case SDLK_RIGHT:
					//alien_x += 1;
					break;
				case SDLK_UP:
					//alien_y -= 1;
					break;
				case SDLK_DOWN:
					//alien_y += 1;
					break;
				case SDLK_q:
					gameRunning = false;
					break;
				case SDLK_ESCAPE:
					gameRunning = false;
					break;
				default:
					break;
				}
			}
		}

		/* Update game */
		system.update();

		/* Render */
		// future

		SDL_Delay(MS_PER_FRAME - (system.getElapsedTime() - startTime));
	}
	
	system.destroy();
	return 0;
}