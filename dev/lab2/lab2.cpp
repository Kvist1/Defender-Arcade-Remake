#define _CRT_SECURE_NO_WARNINGS

#include "SDL.h"
#include "avancezlib.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	AvancezLib system;

	/* Initialise SDL and video modes and all that */
	system.init(640, 480);

	Sprite * foo = system.createSprite("data/enemy_0.bmp");
	float x = 10;

	int num_frames = 0;
	int sum_delta = 0;
	float avg_fps = 0;
	float MAX_FPS = 60;
	char msg[256];

	float vel = 0.1;

	int last_time = system.getElapsedTime();
	while (system.update()) 
	{
		int new_time = system.getElapsedTime();
		int delta = new_time - last_time;
		last_time = new_time;

		sum_delta += delta;

		foo->draw(int(x += vel * delta) % 640, 10);

		AvancezLib::KeyStatus key;
		system.getKeyStatus(key);
		if (key.fire)			SDL_Log("fire!\n");
		if (key.left)			SDL_Log("left!\n");
		if (key.right)			SDL_Log("right!\n");

		num_frames++;

		if (sum_delta > 100)
		{
			avg_fps = ((float)num_frames / sum_delta) * 1000;
			num_frames = 0;
			sum_delta = 0;
		}
		sprintf(msg, "%.3f fps", avg_fps);
		system.drawText(12, 12, msg);
	
		// delay on the loop if needed
		float frameTicks = system.getElapsedTime() - last_time;
		if (1000 / MAX_FPS > frameTicks)
		{
			SDL_Delay(1000 / MAX_FPS - frameTicks);
		}
	} 

	foo->destroy();
	
	system.destroy();

	return 0;
}