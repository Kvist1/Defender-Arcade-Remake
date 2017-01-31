#define _CRT_SECURE_NO_WARNINGS

#include "SDL.h"
#include "avancezlib.h"
#include "stdio.h""
#include "Player.h"
#include "Rocket.h"

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

bool game_over = false;
float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier

int main(int argc, char* argv[])
{
	AvancezLib system;
	Entity* entities[50];
	int numEntities = 50;

	/* Initialise SDL and video modes and all that */
	system.init(640, 480);

	// Create player : system, x, y, vel
	Player player(system, 320-16, 415, 0.6f);
	Sprite * playerSprite = system.createSprite("data/player.bmp");
	entities[0] = &player;

	// Create rocket
	Rocket rocket(system, player, 320, 415, 0.3f);
	Sprite * rocketSprite = system.createSprite("data/rocket.bmp");
	entities[1] = &rocket;

	// Create aliens
	Sprite * alien_0 = system.createSprite("data/enemy_0.bmp");
	Sprite * alien_1 = system.createSprite("data/enemy_1.bmp");
	float alien_x = 10;

	int num_frames = 0;
	int sum_delta = 0;
	int sum_delta_alien_step = 0;
	bool alien_arms_up = true;
	float avg_fps = 0;
	float MAX_FPS = 60;
	int delta = 0;
	char msg[256];

	float alien_vel = 0.1f;
	float ship_vel = 0.1f;

	int last_time = system.getElapsedTime();
	while (system.update()) 
	{
		int new_time = system.getElapsedTime();
		delta = new_time - last_time;
		system.setDelta(delta);
		last_time = new_time;

		sum_delta += delta;
		
		
		for (int i = 0; i < 2; i++)
		{
			entities[i]->update();
		}

		/*player.update();
		rocket.update();*/

		if (alien_arms_up)
		{
			alien_0->draw(int(alien_x += alien_vel * delta) % 640, 10);
			if (sum_delta_alien_step > 30)
			{
				alien_arms_up = false;
				sum_delta_alien_step = 0;
			}
		} 
		else
		{
			alien_1->draw(int(alien_x += alien_vel * delta) % 640, 10);
			if (sum_delta_alien_step > 30)
			{
				alien_arms_up = true;
				sum_delta_alien_step = 0;
			}
		}
		sum_delta_alien_step++;

		playerSprite->draw((int)player.x(), (int)player.y());
		if (rocket.getIsVisible()) rocketSprite->draw((int)rocket.x(), (int)rocket.y());

		//if (key.fire)			
		//	SDL_Log("fire!\n");
		//if (key.left)
		//{
		//	SDL_Log("left! v: %d\n", player.x());
		//	player.setX(player.x() - ship_vel * delta);
		//}
		//if (key.right)
		//{
		//	player.setX(player.x() + ship_vel * delta);
		//	//playerShip->draw(int(ship_x += ship_vel * delta) % 640, ship_y);
		//	//SDL_Log("right!\n");
		//}

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

	alien_0->destroy();
	
	system.destroy();

	return 0;
}