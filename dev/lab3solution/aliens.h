// the aliens' grid
#include <cstdlib>

class Aliens
{
	Sprite * sprite0, *sprite1, *bomb_sprite;
	float horizontalPosition;
	AvancezLib* system;

	float direction;	// 1: aliens go right, -1: aliens go left

public:

	Alien * grid;
	CircularArray<Bomb> bombs;
	float time_bomb_launched;


	void Init(AvancezLib* system)
	{
		this->system = system;
		sprite0 = system->createSprite("data/enemy_0.bmp");
		sprite1 = system->createSprite("data/enemy_1.bmp");
		horizontalPosition = 320;

		// init a grid of aliens 5 x 11
		grid = new Alien[55];
		for (auto i = 0; i < 5; i++)
		for (auto j = 0; j < 11; j++)
			grid[i * 11 + j].Init(j * 38 + 111, i * 32 + 32 * 2);
		direction = 1;

		bombs.Allocate(MAX_NUM_BOMBS);
		bomb_sprite = system->createSprite("data/bomb.bmp");
		time_bomb_launched = -10000.f;	// time fromthe last time a bomb was dropped by one of the aliens
	}


	void Update(float dt)
	{
		bool reached_edge_screen = false;
		for (auto i = 0; i < 55; i++)
		if (grid[i].enabled)
			reached_edge_screen |= grid[i].Update(direction, dt);

		if (reached_edge_screen)
		{
			direction *= -1.f;	// invert direction

			// go down 32 px
			for (auto i = 0; i < 55; i++)
			if (grid[i].enabled)
				grid[i].GoDown();
		}

		bombs.Update(dt);

		// shoot the bomb
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_bomb_launched) < BOMB_TIME_INTERVAL)
			return;

		for (auto i = 0; i < 55; i++)
		if (grid[i].enabled)
		{
			// decide randomly if the bomb should be dropped
			if (rand() >(RAND_MAX * (1.f - 1.f / 45.f)))
			{
				Bomb * bomb = (Bomb *)bombs.GetFirstAvailableElem();
				bomb->Init(grid[i].horizontalPosition, grid[i].verticalPosition);
			}
		}
		time_bomb_launched = system->getElapsedTime();
	}

	// collision handling betwee the rockets thrown by the player and the aliens in the grid
	void TestCollisionsWthRockets(Player * player)
	{
		CircularArray<Rocket> * rockets = &(player->rockets);

		int nb_hits = 0;
		for (auto i = 0; i < 55; i++)
		if (grid[i].enabled)
		{
			for (auto j = 0; j < MAX_NUM_ROCKETS; j++)
			{
				Rocket * rocket = (Rocket *)rockets->GetAt(j);
				if (rocket->enabled)
				{
					// test rocket - alien collision 
					if ((rocket->horizontalPosition > grid[i].horizontalPosition - 10) &&
						(rocket->horizontalPosition < grid[i].horizontalPosition + 10) &&
						(rocket->verticalPosition   > grid[i].verticalPosition - 10) &&
						(rocket->verticalPosition < grid[i].verticalPosition + 10))
					{
						// if hit, remove rocket and alien
						rocket->Shutdown();
						grid[i].Shutdown();

						// ... and increase the score
						player->score += POINTS_PER_ALIEN * game_speed;
					}
				}
			}
		}
		else
			nb_hits++;

		if (nb_hits == 55)	// if all the aliens have been hit
		{
			// reinit the grid of the aliens on top of the screen
			for (auto i = 0; i < 5; i++)
			for (auto j = 0; j < 11; j++)
				grid[i * 11 + j].Init(j * 38 + 111, i * 32 + 32 * 2);

			// increase the speed of the game
			// each time the speed is increased, the game becomes a bit more difficult 
			// and the amount of scored points increases
			game_speed += 0.2f;
			direction = 1.f;
		}
	}

	void Draw()
	{
		float t0 = system->getElapsedTime();
		bombs.Draw(bomb_sprite);  // draw the bombs
		for (auto i = 0; i < 55; i++)
		if (grid[i].enabled)
		if (((int)t0 % 2) == 0)
			grid[i].Draw(sprite0);
		else
			grid[i].Draw(sprite1);

	}

	void Destroy()
	{
		sprite0->destroy();
		sprite1->destroy();
		bomb_sprite->destroy();

		delete[] grid;
		bombs.Deallocate();
	}
};
