
bool change_direction = false;

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* system;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	ObjectPool<Alien> aliens_pool;
	ObjectPool<Bomb> bombs_pool;
	ObjectPool<Human> humans_pool;

	Player * player;
	AliensGrid * aliens_grid;

	Sprite * life_sprite;
	bool game_over;

	unsigned int score = 0;

	// bg 1920*480px, mini 344*85px
	Sprite *bg_sprite, *bgMini_sprite;
	//The camera area
	SDL_Rect camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	int bgScrollingOffset = 0;
	int showGameOverBlink = 0;

public:

	virtual void Create(AvancezLib* system)
	{
		SDL_Log("Game::Create");

		this->system = system;
		bg_sprite = system->createSprite("data/test_background.bmp");
		bgMini_sprite = system->createSprite("data/test_background_small2.bmp");

		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(system, player, &game_objects, &rockets_pool);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(system, player, &game_objects, "data/player_left.bmp", "data/player_right.bmp");
		CollideComponent * player_bomb_collision = new CollideComponent();
		player_bomb_collision->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&bombs_pool);
		CollideComponent * player_alien_collision = new CollideComponent();
		player_alien_collision->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&aliens_pool);

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_bomb_collision);
		player->AddComponent(player_alien_collision);
		player->AddReceiver(this);
		game_objects.insert(player);

		rockets_pool.Create(30);
		for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
		{
			RocketBehaviourComponent * behaviour = new RocketBehaviourComponent();
			behaviour->Create(system, *rocket, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *rocket, &game_objects, "data/rocket.bmp", "data/rocket.bmp");
			(*rocket)->Create();
			(*rocket)->AddComponent(behaviour);
			(*rocket)->AddComponent(render);
		}

		humans_pool.Create(4);
		for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++)
		{
			HumanBehaviourComponent * behaviour = new HumanBehaviourComponent();
			behaviour->Create(system, *human, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *human, &game_objects, "data/human.bmp", "data/human.bmp");
			CollideComponent * rocket_coll = new CollideComponent();
			rocket_coll->Create(system, *human, &game_objects, (ObjectPool<GameObject>*)&rockets_pool);
			(*human)->AddComponent(behaviour);
			(*human)->AddComponent(render);
			(*human)->AddComponent(rocket_coll);
			game_objects.insert(*human);
		}


		aliens_grid = new AliensGrid();
		AliensGridBehaviourComponent  * aliensgrid_behaviour = new AliensGridBehaviourComponent();
		aliensgrid_behaviour->Create(system, aliens_grid, &game_objects, &aliens_pool, &bombs_pool);
		aliens_grid->Create();
		aliens_grid->AddComponent(aliensgrid_behaviour);
		game_objects.insert(aliens_grid);


		aliens_pool.Create(55);
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
		{
			AlienBehaviourComponent * alien_behaviour = new AlienBehaviourComponent();
			alien_behaviour->Create(system, *alien, &game_objects);
			RenderComponent * alien_render = new RenderComponent();
			alien_render->Create(system, *alien, &game_objects, "data/enemy_0.bmp");
			CollideComponent * alien_coll = new CollideComponent();
			alien_coll->Create(system, *alien, &game_objects, (ObjectPool<GameObject>*)&rockets_pool);
			(*alien)->Create();
			(*alien)->AddComponent(alien_behaviour);
			(*alien)->AddComponent(alien_render);
			(*alien)->AddComponent(alien_coll);
			(*alien)->AddReceiver(this);
		}

		bombs_pool.Create(55);
		for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent * bomb_behaviour = new BombBehaviourComponent();
			bomb_behaviour->Create(system, *bomb, &game_objects);
			RenderComponent * bomb_render = new RenderComponent();
			bomb_render->Create(system, *bomb, &game_objects, "data/bomb.bmp");

			(*bomb)->Create();
			(*bomb)->AddComponent(bomb_behaviour);
			(*bomb)->AddComponent(bomb_render);
		}

		life_sprite = system->createSprite("data/player_left.bmp");
		score = 0;
	}

	virtual void Init()
	{
		player->Init();
		aliens_grid->Init();
		for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++) 
			(*human)->Init();

		enabled = true;
		game_over = false;
	}

	virtual void Update(float dt)
	{
		if (IsGameOver())
			dt = 0.f;

		UpdateBackground(camera.x, camera.y);
		UpdateCamera();

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt, camera.x, camera.y);

		// check if there are still active aliens
		// if not, send a message to re-init the level
		bool are_aliens_still_there = false;
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
			are_aliens_still_there |= (*alien)->enabled;
		if (!are_aliens_still_there)
		{
			// level win!
			game_speed += 0.4f;
			aliens_grid->Init();
		}

		// check if all humans are dead
		// if they are, game over
		bool humans_are_still_alive = false;
		for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++)
			humans_are_still_alive |= (*human)->enabled;
		if (!humans_are_still_alive)
		{
			game_over = true;
		}

	}

	virtual void UpdateBackground(int camX, int camY)
	{
		if (camX > LEVEL_WIDTH)
			camX = 0;
		else if (camX < -camera.w)
			camX = LEVEL_WIDTH - camera.w;

		bg_sprite->draw(-camX - LEVEL_WIDTH, 0);
		bg_sprite->draw(-camX, 0);
		bg_sprite->draw(-camX + LEVEL_WIDTH, 0);

		float scaling = LEVEL_WIDTH / MINIMAP_WIDTH;
		float miniMapCamX = (float)camX / scaling;
		/*bgMini_sprite->draw(WINDOW_WIDTH / 2 - MINIMAP_WIDTH / 2 - miniMapCamX - MINIMAP_WIDTH, 0);
		bgMini_sprite->draw(WINDOW_WIDTH/2 - MINIMAP_WIDTH/2 - miniMapCamX, 0);
		bgMini_sprite->draw(WINDOW_WIDTH / 2 - MINIMAP_WIDTH / 2 - miniMapCamX + MINIMAP_WIDTH, 0);*/
		bgMini_sprite->draw(WINDOW_WIDTH / 2 - 53 - miniMapCamX - MINIMAP_WIDTH, 0);
		bgMini_sprite->draw(WINDOW_WIDTH / 2 - 53 - miniMapCamX, 0);
		bgMini_sprite->draw(WINDOW_WIDTH / 2 - 53 - miniMapCamX + MINIMAP_WIDTH, 0);

		SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - 53 + player->horizontalPosition / 6 - miniMapCamX + 16/6, player->verticalPosition / 6 + 16/6);
	}

	virtual void UpdateCamera()
	{
		//Center the camera over the palyer. 32/2 is the with of the player sprite
		camera.x = (player->horizontalPosition + 32 / 2) - WINDOW_WIDTH / 2;
		camera.y = (player->verticalPosition + 32 / 2) - WINDOW_HEIGHT / 2;

		//Keep the camera in bounds
		/*if (camera.x < -640)
		{
			camera.x = -640;
		}*/
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		/*if (camera.x > LEVEL_WIDTH - camera.w)
		{
			camera.x = LEVEL_WIDTH - camera.w;
		}*/
		if (camera.y > LEVEL_HEIGHT - camera.h)
		{
			camera.y = LEVEL_HEIGHT - camera.h;
		}
	}

	virtual void Draw()
	{
		char msg[1024];
		sprintf(msg, "Score: %07d", Score());
		system->drawText(510, 16, msg);
		sprintf(msg, "Bonus: %.1fX", game_speed);
		system->drawText(510, 40, msg);

		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i*36+20, 16);

		if (IsGameOver())
		{
			sprintf(msg, "*** G A M E  O V E R ***");

			if (showGameOverBlink == 0)
			{
				system->drawText(250, 250, msg);
				SDL_Delay(600.0f);
				showGameOverBlink = 1;
			} 
			else if (showGameOverBlink == 1)
			{
				SDL_Delay(800.0f);
				showGameOverBlink = 0;
			}
		}
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
			game_over = true;

		if (m == ALIEN_HIT)
			score += POINTS_PER_ALIEN * game_speed;
	}


	bool IsGameOver()
	{
		return game_over;
	}

	unsigned int Score()
	{
		return score;
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();
	
		rockets_pool.Destroy();
		aliens_pool.Destroy();
		bombs_pool.Destroy();

		delete aliens_grid;
		delete player;
	}
};