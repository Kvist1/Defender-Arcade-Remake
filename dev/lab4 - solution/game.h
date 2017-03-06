
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
	Sprite * bomb_count_sprite;
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
		bgMini_sprite = system->createSprite("data/test_background_small.bmp");

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
		aliensgrid_behaviour->Create(system, aliens_grid, player, &game_objects, &aliens_pool, &bombs_pool);
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

		bombs_pool.Create(100);
		for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent * bomb_behaviour = new BombBehaviourComponent();
			bomb_behaviour->Create(system, *bomb, &game_objects);
			RenderComponent * bomb_render = new RenderComponent();
			bomb_render->Create(system, *bomb, &game_objects, "data/bomb.bmp", "data/bomb.bmp");

			(*bomb)->Create();
			(*bomb)->AddComponent(bomb_behaviour);
			(*bomb)->AddComponent(bomb_render);
		}

		life_sprite = system->createSprite("data/player_left.bmp");
		bomb_count_sprite = system->createSprite("data/player_bomb.bmp");
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

		DrawBackground(camera.x, camera.y);
		DrawMiniMap(camera.x, camera.y);
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
			GAME_SPEED += 0.4f;
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

	virtual void DrawBackground(int camX, int camY)
	{
		if (camX > LEVEL_WIDTH)
			camX = 0;
		else if (camX < -camera.w)
			camX = LEVEL_WIDTH - camera.w;

		bg_sprite->draw(-camX - LEVEL_WIDTH, 0);
		bg_sprite->draw(-camX, 0);
		bg_sprite->draw(-camX + LEVEL_WIDTH, 0);
	}

	virtual void DrawMiniMap(int camX, int camY)
	{
		int scaling = LEVEL_WIDTH / MINIMAP_WIDTH;
		int mCamX = camX / scaling; 
		int mWindowWidth = WINDOW_WIDTH / scaling;

		/* 
			creates 3 continous copies of the minimap (level), to simulate the loop/warp
			 		 ____________________________________
			 	<-	|   left    |   middle   |   right   |  ->
				  	|___________|____________|___________|
									|	|
								 camera width

			moves left and right depending on camera x position
		*/
		int mini_map_x_position = WINDOW_WIDTH / 2 - mWindowWidth / 2 - mCamX;
		bgMini_sprite->draw( mini_map_x_position - MINIMAP_WIDTH, 0);
		bgMini_sprite->draw( mini_map_x_position, 0);
		bgMini_sprite->draw( mini_map_x_position + MINIMAP_WIDTH, 0);

		int halfSpriteSize = 32/2; // the sprites are 32px
		int mXPos = WINDOW_WIDTH / 2 - mWindowWidth / 2 + player->position.x / scaling - mCamX + halfSpriteSize / scaling;
		int mYPos = player->position.y / scaling + halfSpriteSize / scaling;
		SDL_SetRenderDrawColor(system->renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(system->renderer, mXPos, mYPos);
		SDL_RenderDrawPoint(system->renderer, mXPos+1, mYPos);
		SDL_RenderDrawPoint(system->renderer, mXPos-1, mYPos);
		SDL_RenderDrawPoint(system->renderer, mXPos, mYPos+1);
		SDL_RenderDrawPoint(system->renderer, mXPos, mYPos-1);


		SDL_SetRenderDrawColor(system->renderer, 50, 50, 255, 255);
		for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++)
			if ((*human)->enabled)
			{
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*human)->position.x / scaling - mCamX + 16 / scaling, (*human)->position.y / scaling + 16 / scaling);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*human)->position.x / scaling - mCamX + 16 / scaling+1, (*human)->position.y / scaling + 16 / scaling);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*human)->position.x / scaling - mCamX + 16 / scaling-1, (*human)->position.y / scaling + 16 / scaling);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*human)->position.x / scaling - mCamX + 16 / scaling, (*human)->position.y / scaling + 16 / scaling+1);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*human)->position.x / scaling - mCamX + 16 / scaling, (*human)->position.y / scaling + 16 / scaling-1);
			}
		
		SDL_SetRenderDrawColor(system->renderer, 255, 0, 0, 255);
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
			if ((*alien)->enabled)
			{
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*alien)->position.x / scaling - mCamX + 16 / scaling, (*alien)->position.y / scaling + 16 / scaling);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*alien)->position.x / scaling - mCamX + 16 / scaling+1, (*alien)->position.y / scaling + 16 / scaling);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*alien)->position.x / scaling - mCamX + 16 / scaling-1, (*alien)->position.y / scaling + 16 / scaling);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*alien)->position.x / scaling - mCamX + 16 / scaling, (*alien)->position.y / scaling + 16 / scaling+1);
				SDL_RenderDrawPoint(system->renderer, WINDOW_WIDTH / 2 - mWindowWidth / 2 + (*alien)->position.x / scaling - mCamX + 16 / scaling, (*alien)->position.y / scaling + 16 / scaling-1);

			}
	
		SDL_Rect rect = { 0, 0, 160, 80 };
		SDL_Rect rect2 = { 160+320, 0, 160, 80 };

		SDL_Rect camera_left_edge = { 320-54, 5, 1, 70 };
		SDL_Rect camera_left_edge_top_wing = { 320 - 54, 5, 10, 1 };
		SDL_Rect camera_left_edge_bottom_wing = { 320 - 54, 75, 10, 1 };

		SDL_Rect camera_right_edge = { 320+54, 5, 1, 70 };
		SDL_Rect camera_right_edge_top_wing = { 320 + 44, 5, 10, 1 };
		SDL_Rect camera_right_edge_bottom_wing = { 320 + 45, 75, 10, 1 };

		SDL_SetRenderDrawColor(system->renderer, 188, 188, 188, 255);

		SDL_RenderFillRect(system->renderer, &rect);
		SDL_RenderFillRect(system->renderer, &rect2);
		SDL_RenderFillRect(system->renderer, &camera_left_edge);
		SDL_RenderFillRect(system->renderer, &camera_left_edge_top_wing);
		SDL_RenderFillRect(system->renderer, &camera_left_edge_bottom_wing);
		SDL_RenderFillRect(system->renderer, &camera_right_edge);
		SDL_RenderFillRect(system->renderer, &camera_right_edge_top_wing);
		SDL_RenderFillRect(system->renderer, &camera_right_edge_bottom_wing);
		SDL_SetRenderDrawColor(system->renderer, 255, 255, 255, 255);
	}

	virtual void UpdateCamera()
	{
		//Center the camera over the player. 32/2 is the with of the player sprite
		camera.x = (player->position.x + 32 / 2) - WINDOW_WIDTH / 2;
		camera.y = (player->position.y + 32 / 2) - WINDOW_HEIGHT / 2;

		//Keep the camera in bounds
		if (camera.y < 0)
		{
			camera.y = 0;
		}
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
		sprintf(msg, "Bonus: %.1fX", GAME_SPEED);
		system->drawText(510, 40, msg);

		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i*36+20, 5);

		for (int i = 0; i < player->bombs; i++)
			bomb_count_sprite->draw(i * 36 + 20, 40);

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
			score += POINTS_PER_ALIEN * GAME_SPEED;

		if (m == PLAYER_BOMB_DROPPED)
			KillAllAliensInRange();
	}

	void KillAllAliensInRange()
	{
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
			if (	(*alien)->enabled 
				&&	(*alien)->position.x >= camera.x
				&&	(*alien)->position.x <= camera.x + camera.w )
			{
				(*alien)->Receive(HIT);
			}
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
		bg_sprite->destroy();
		bgMini_sprite->destroy();
		bomb_count_sprite->destroy();
	
		rockets_pool.Destroy();
		aliens_pool.Destroy();
		bombs_pool.Destroy();
		humans_pool.Destroy();

		delete aliens_grid;
		delete player;
	}
};