#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"

#include "GlobalVariables.h"	
const unsigned int	NUM_LIVES = 2;
const unsigned int	MAX_NUM_ROCKETS = 32;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 100;
const float			FIRE_TIME_INTERVAL = .5f;
const float			BOMB_TIME_INTERVAL = 1.25f;
const float			PLAYER_SPEED = 160.0f;
const float			ROCKET_SPEED = 160.0f;
const float			ALIEN_SPEED = 40.0f;
const float			BOMB_SPEED = 120.0f;
const float			MAX_FPS = 40.0f;

bool game_over = false;
// speed of the game; it is increased each time all the aliens are hit
// it is also the score multiplier
float game_speed = 1.f;


//#include "circular_array.h"

//#include "projectile.h"
//#include "rocket.h"
//#include "bomb.h"
//#include "alien.h"

#include "player.h"
//#include "aliens.h"

int main(int argc, char** argv)
{
	// fps counter
	int num_frames = 0;
	float avg_fps = 0;
	float sum_delta = 0;
	float dt;

	AvancezLib system;
	system.init(640, 480);
	Graphics* graphics = system.createGraphics();

	World world;

	Player player;
	player.Init(&system);

	//Aliens aliens;
	//aliens.Init(&system);

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		float newTime = system.getElapsedTime();
		dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		if (game_over)
			dt = 0.f;

		player.Update(world);
		//aliens.Update(dt);

		// bad encapsulation: however in this case it is the most practical approach
		/*player.TestCollisionsWthAliens(aliens.grid);
		player.TestCollisionsWthBombs(&(aliens.bombs));
		aliens.TestCollisionsWthRockets(&player);*/

		//player.Draw(); //movede to player
		//aliens.Draw();

		char msg[1024];
		sprintf(msg, "%07d", player.score);
		system.drawText(300, 32, msg);
		sprintf(msg, "bonus: %.1fX", game_speed);
		system.drawText(510, 32, msg);

		if (game_over)
		{
			sprintf(msg, "*** G A M E  O V E R ***");
			system.drawText(250, 8, msg);
		}

		// check fps 
		num_frames++;
		sum_delta += dt/game_speed;
		if (sum_delta > 1)
		{
			avg_fps = ((float)num_frames / sum_delta);
			num_frames = 0;
			sum_delta = 0;
		}
		sprintf(msg, "%.1f fps", avg_fps);
		system.drawText(12, 12, msg);

		// delay on the loop if needed
		float frameTicks = system.getElapsedTime() - lastTime;
		if (1000.0f / MAX_FPS > frameTicks)
		{
			SDL_Delay(1000.0f / MAX_FPS - frameTicks);
		}
	}

	//aliens.Destroy();
	player.Destroy();
	system.destroy();

	return 0;
}



