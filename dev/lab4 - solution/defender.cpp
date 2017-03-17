#define _CRT_SECURE_NO_WARNINGS

#include "GlobalVariables.h"
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int LEVEL_WIDTH = 3840;
const int LEVEL_HEIGHT = WINDOW_HEIGHT;

const int MINIMAP_WIDTH = LEVEL_WIDTH/6;
const int MINIMAP_HEIGHT = LEVEL_HEIGHT/6;

const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 5;
const unsigned int	NUM_PLAYER_BOMBS = 3;
const unsigned int	MAX_NUM_ROCKETS = 32;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 100;
const float			FIRE_TIME_INTERVAL = .4f;
const float			PLAYER_BOMB_TIME_INTERVAL = 2.0f;
const float			BOMB_TIME_INTERVAL = 3.25f;
const float			ALIEN_RANGE = 400.0f;
const float			PLAYER_SPEED = 550.0f;
const float			ROCKET_SPEED = 1200.0f;
const float			ALIEN_SPEED = 160.0f;
const float			BOMB_SPEED = 500.0f;
const float			HUMAN_SPEED = 30.0f;

float GAME_SPEED = 1.f;		// speed of the game; it is also the score multiplier

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"
#include <set>

#include "component.h"
#include "game_object.h"
#include "rocket.h"
#include "bomb.h"
#include "player.h"
#include "human.h"
#include "alien.h"
#include "power_up.h"
#include "aliens_grid.h"
#include "game.h"
#include "SDL_image.h"
#include "MiniMap.h"

int main(int argc, char** argv)
{
	
	AvancezLib system;
	AvancezLib::SystemState gameState;

	system.init(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	Game game;
	game.Create(&system);
	game.Init();

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		
		float newTime = system.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * GAME_SPEED;
		lastTime = newTime;

		system.getSystemState(gameState);
		if (gameState.isPaused)
			game.Update(0);
		else
			game.Update(dt);

		game.Draw();
	}

	// clean up
	game.Destroy();
	system.destroy();

	return 0;
}



