#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"
#include "Box2D\Box2D.h"

#include <set>

#include "GlobalVariables.h"
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = WINDOW_HEIGHT;

const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 2;

const unsigned int	MAX_NUM_ROCKETS = 32;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 100;
const float			FIRE_TIME_INTERVAL = .4f;
const float			BOMB_TIME_INTERVAL = 1.25f;
const float			PLAYER_SPEED = 350.0f;
const float			ROCKET_SPEED = 1000.0f;
const float			ALIEN_SPEED = 70.0f;
const float			BOMB_SPEED = 120.0f;
const float			HUMAN_SPEED = 120.0f;


float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier


#include "component.h"
#include "game_object.h"


//#include "collision_system.h"


#include "rocket.h"
#include "bomb.h"
#include "alien.h"
#include "aliens_grid.h"
#include "player.h"
#include "human.h"
#include "game.h"
#include "Box2D\Box2D.h"

int main(int argc, char** argv)
{
	// fps counter
	int num_frames = 0;
	float avg_fps = 0;
	float sum_delta = 0;
	float MAX_FPS = 71.0f;
	bool checkFPS = false;
	
	AvancezLib system;
	AvancezLib::SystemState gameStates;

	system.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	Game game;
	game.Create(&system);
	game.Init();

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		float newTime = system.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		system.getSystemState(gameStates);
		if (gameStates.isPaused)
			game.Update(0);
		else
			game.Update(dt);

		game.Draw();



		// check fps 
		if (checkFPS)
		{
			num_frames++;
			sum_delta += dt / game_speed;
			if (sum_delta > 1)
			{
				avg_fps = ((float)num_frames / sum_delta);
				num_frames = 0;
				sum_delta = 0;
			}
			char msg[1024];
			sprintf(msg, "%.1f fps", avg_fps);
			system.drawText(12, 12, msg);

			// delay on the loop if needed
			float frameTicks = system.getElapsedTime() - lastTime;
			if (1000.0f / MAX_FPS > frameTicks)
			{
				SDL_Delay(1000.0f / MAX_FPS - frameTicks);
			}
		}
	}

	// clean up
	game.Destroy();
	system.destroy();

	return 0;
}



