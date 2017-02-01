

// the main player
class Player
{
	Sprite * sprite, *rocket_sprite;
	AvancezLib* system_;

	float time_fire_pressed;	// time from the last time the fire button was pressed

public:
	CircularArray<Rocket> rockets;
	int lives;	// it's game over when goes below zero 
	float horizontalPosition;
	float verticalPosition;

	int score;

	void Init(AvancezLib* system)
	{
		this->system_ = system;
		sprite = system->createSprite("data/player.bmp");
		horizontalPosition = 320;
		verticalPosition = 480 - 32;

		rockets.Allocate(MAX_NUM_ROCKETS);
		rocket_sprite = system->createSprite("data/rocket.bmp");
		time_fire_pressed = -10000.f;

		lives = NUM_LIVES;
		score = 0;
		game_over = false;
	}


	void Update(float dt)
	{
		rockets.Update(dt);
	}

	// collision handling between the bombs thrown by the aliens and the player
	void TestCollisionsWthBombs(CircularArray<Bomb> * bombs)
	{
		for (auto j = 0; j < MAX_NUM_BOMBS; j++)
		{
			Bomb * bomb = (Bomb *)bombs->GetAt(j);
			if (bomb->enabled)
			{
				// test bombs - alien collision 
				if ((bomb->horizontalPosition > horizontalPosition - 10) &&
					(bomb->horizontalPosition < horizontalPosition + 10) &&
					(bomb->verticalPosition   > verticalPosition - 10) &&
					(bomb->verticalPosition < verticalPosition + 10))
				{
					// if the player is hit, remove the bomb and one life
					bomb->Shutdown();
					RemoveLife();
				}
			}
		}
	}

	// collision handling between the aliens and the player
	void TestCollisionsWthAliens(Alien * grid)
	{
		for (auto i = 0; i < 55; i++)
		if (grid[i].enabled)
		if ((grid[i].horizontalPosition > horizontalPosition - 10) &&
			(grid[i].horizontalPosition < horizontalPosition + 10) &&
			(grid[i].verticalPosition   > verticalPosition - 10) &&
			(grid[i].verticalPosition   < verticalPosition + 10))
		{
			// if the player is hit, remove the alien and one life
			grid[i].Shutdown();
			RemoveLife();
		}
	}

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		horizontalPosition += move;

		if (horizontalPosition > (640 - 32))
			horizontalPosition = 640 - 32;

		if (horizontalPosition < 0)
			horizontalPosition = 0;
	}

	// fire a rocket if enough time has passed from the previous one
	void Fire()
	{
		// shoot just if enough time passed by
		if ((system_->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
			return;

		// use the first rocket available
		Rocket * rocket = (Rocket *)rockets.GetFirstAvailableElem();
		rocket->Init(horizontalPosition, 480 - 52);
		time_fire_pressed = system_->getElapsedTime();
	}


	void Draw()
	{
		sprite->draw(int(horizontalPosition), verticalPosition);

		rockets.Draw(rocket_sprite);

		// draw lives on the upper left side of the creen
		for (auto i = 0; i < lives; i++)
			sprite->draw(32 * i, 24);
	}

	void RemoveLife()
	{
		lives--;
		if (lives < 0)
			game_over = true;
	}

	void Destroy()
	{
		sprite->destroy();
		rocket_sprite->destroy();

		rockets.Deallocate();
	}
};
