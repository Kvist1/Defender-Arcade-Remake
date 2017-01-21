#include "SDL.h"
#include "avancezlib.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	AvancezLib system;
	SDL_Event event;
	bool gameRunning = true;
	int startTicks;
	int MS_PER_FRAME = 16;
	int MAX_FPS = 60;

	/* Initialise SDL and video modes and all that */
	system.init(900, 640);


	/* Main game loop */
	while (gameRunning) 
	{

		startTicks = system.getElapsedTime();

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
					SDL_Log("Left");
					break;
				case SDLK_RIGHT:
					//alien_x += 1;
					SDL_Log("Right");
					break;
				case SDLK_SPACE:
					//alien_y -= 1;
					SDL_Log("Fire");
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


		int frameTicks = system.getElapsedTime() - startTicks;

		// calculate fps only every 100 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 100)
		{
			system.calculateFPS(MAX_FPS, frameTicks);
			frameCounter = 0;
		}

		// delay on the loop if needed
		frameTicks = system.getElapsedTime() - startTicks;
		if (1000 / MAX_FPS > frameTicks)
		{
			SDL_Delay(1000 / MAX_FPS - frameTicks);
		}


	} // end main game loop
	
	system.destroy();
	return 0;
}