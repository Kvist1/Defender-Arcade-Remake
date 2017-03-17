#include "MiniMap.h"
#include "ExplosionRenderHandler.h"

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

	ExplosionRenderHandler *explosionHandler;
	MiniMapBackgroundHandler *mmbHandler;
	Sprite *bg_sprite;
	//The camera area
	SDL_Rect camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	int bgScrollingOffset = 0;
	int showGameOverBlink = 0;

	enum GameState
	{
		normal,
		abduction
	};
	GameState gameState;

public:

	virtual void Create(AvancezLib* system)
	{
		SDL_Log("Game::Create");

		//create system
		this->system = system;
		bg_sprite = system->createSprite("data/test_background.bmp");

		//create minimap background
		mmbHandler = new MiniMapBackgroundHandler();
		mmbHandler->Create(system, "data/test_background_small.bmp");

		//create explosion handler
		explosionHandler = new ExplosionRenderHandler();
		explosionHandler->Create(system);

		//create player
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(system, player, &game_objects, &rockets_pool);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(system, player, &game_objects, "data/player_left.png", "data/player_right.png");
		MiniMapRenderComponent * player_miniRender = new MiniMapRenderComponent();
		player_miniRender->Create(system, player, &game_objects, "data/player_minimap.png");
		CollideComponent * player_bomb_collision = new CollideComponent();
		player_bomb_collision->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&bombs_pool);
		CollideComponent * player_alien_collision = new CollideComponent();
		player_alien_collision->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&aliens_pool);
		
		player->Create(system->getSurfaceSize("data/player_left.png"));
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_miniRender);
		player->AddComponent(player_bomb_collision);
		player->AddComponent(player_alien_collision);
		player->AddReceiver(this);
		game_objects.insert(player);

		//create rockets
		rockets_pool.Create(30);
		for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
		{
			RocketBehaviourComponent * behaviour = new RocketBehaviourComponent();
			behaviour->Create(system, *rocket, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *rocket, &game_objects, "data/player_rocket_left.png", "data/player_rocket_right.png");
			
			(*rocket)->Create(system->getSurfaceSize("data/player_rocket_left.png"));
			(*rocket)->AddComponent(behaviour);
			(*rocket)->AddComponent(render);
		}

		//create alien handler 
		aliens_grid = new AliensGrid();
		AliensGridBehaviourComponent  * aliensgrid_behaviour = new AliensGridBehaviourComponent();
		aliensgrid_behaviour->Create(system, aliens_grid, player, &game_objects, &aliens_pool, &bombs_pool, &humans_pool);
		aliens_grid->Create();
		aliens_grid->AddComponent(aliensgrid_behaviour);
		game_objects.insert(aliens_grid);

		//create aliens
		aliens_pool.Create(55, system->getSurfaceSize("data/Alien_Ship.png") );
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
		{
			AlienBehaviourComponent * alien_behaviour = new AlienBehaviourComponent();
			alien_behaviour->Create(system, *alien, &game_objects);
			RenderComponent * alien_render = new RenderComponent();
			alien_render->Create(system, *alien, &game_objects, "data/Alien_Ship.png");
			MiniMapRenderComponent * alien_miniRender = new MiniMapRenderComponent();
			alien_miniRender->Create(system, (*alien), &game_objects, "data/alien_minimap.png");
			CollideComponent * alien_coll = new CollideComponent();
			alien_coll->Create(system, *alien, &game_objects, (ObjectPool<GameObject>*)&rockets_pool);
			(*alien)->Create();
			(*alien)->AddComponent(alien_behaviour);
			(*alien)->AddComponent(alien_render);
			(*alien)->AddComponent(alien_miniRender);
			(*alien)->AddComponent(alien_coll);
			(*alien)->AddReceiver(this);
		}

		//create humans
		humans_pool.Create(4, system->getSurfaceSize("data/human.png"));
		for (auto human = humans_pool.pool.begin(); human != humans_pool.pool.end(); human++)
		{
			HumanBehaviourComponent * behaviour = new HumanBehaviourComponent();
			behaviour->Create(system, *human, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(system, *human, &game_objects, "data/human.png", "data/human.png");
			MiniMapRenderComponent * human_miniRender = new MiniMapRenderComponent();
			human_miniRender->Create(system, (*human), &game_objects, "data/human_minimap.png");
			CollideComponent * rocket_coll = new CollideComponent();
			rocket_coll->Create(system, *human, &game_objects, (ObjectPool<GameObject>*)&rockets_pool);
			(*human)->AddComponent(behaviour);
			(*human)->AddComponent(render);
			(*human)->AddComponent(rocket_coll);
			(*human)->AddComponent(human_miniRender);
			(*human)->AddReceiver(this);
			(*human)->AddReceiver(aliens_grid);
			game_objects.insert(*human);
		}

		//create alien bombs
		bombs_pool.Create(100, system->getSurfaceSize("data/alien_bomb.png") );
		for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent * bomb_behaviour = new BombBehaviourComponent();
			bomb_behaviour->Create(system, *bomb, &game_objects);
			RenderComponent * bomb_render = new RenderComponent();
			bomb_render->Create(system, *bomb, &game_objects, "data/alien_bomb.png", "data/alien_bomb.png");

			(*bomb)->Create();
			(*bomb)->AddComponent(bomb_behaviour);
			(*bomb)->AddComponent(bomb_render);
		}

		life_sprite = system->createSprite("data/player_life.png");
		bomb_count_sprite = system->createSprite("data/player_bomb.png");
		score = 0;
		gameState = normal;
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
		mmbHandler->DrawMiniMap(camera.x);
		UpdateCamera();

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt, camera.x, camera.y);

		mmbHandler->DrawMiniMapCameraBox();
		explosionHandler->Update(camera.x, dt);

		// check if there are still active aliens
		// if not, send a message to re-init the level
		bool are_aliens_still_there = false;
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
			are_aliens_still_there |= (*alien)->enabled;
		if (!are_aliens_still_there)
		{
			// level win!
			//GAME_SPEED += 0.4f;
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
		system->drawText(1050, 40, msg);
		sprintf(msg, "Bonus: %.1fX", GAME_SPEED);
		system->drawText(1050, 80, msg);

		sprintf(msg, "Lives: ", GAME_SPEED);
		system->drawText(50, 40, msg);
		sprintf(msg, "Bombs: ", GAME_SPEED);
		system->drawText(50, 80, msg);
		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i * 36 + 110, 40-10);

		for (int i = 0; i < player->bombs; i++)
			bomb_count_sprite->draw(i * 36 + 110, 80-10);

		if (gameState == abduction)
		{
			sprintf(msg, "Abduction in process!");
			system->drawText(WINDOW_WIDTH/2 -80, MINIMAP_HEIGHT + 10, msg);
		}

		if (IsGameOver())
		{
			sprintf(msg, "*** G A M E  O V E R ***");

			if (showGameOverBlink == 0)
			{
				system->drawText(550, 400, msg);
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

	virtual void Receive(Package *p)
	{
		if (p->msg == GAME_OVER)
			game_over = true;

		if (p->msg == ALIEN_HIT)
		{
			score += POINTS_PER_ALIEN * GAME_SPEED;
			if (p->position != NULL)
			{
				explosionHandler->AddExplosion(p->position->x, p->position->y);
			}
		}

		if (p->msg == PLAYER_BOMB_DROPPED)
			KillAllAliensInRange();

		if (p->msg == GAME_ABDUCTION)
			gameState = abduction;

		if (p->msg == HUMAN_LOST_IN_SPACE || p->msg == HUMAN_FALLING || p->msg == HUMAN_HIT_WHILE_ABDUCTION)
			gameState = normal;
	}

	void KillAllAliensInRange()
	{
		for (auto alien = aliens_pool.pool.begin(); alien != aliens_pool.pool.end(); alien++)
		{
			if ((*alien)->enabled)
			{
				// normal case, check if alien is inside camera/window view
				if (	(*alien)->position.x >= camera.x
					&&	(*alien)->position.x <= camera.x + camera.w )
				{
					(*alien)->Receive(new Package(HIT));
				}
				// special case if camera going over left edge of level
				else if (	camera.x < 0
						&&	(*alien)->position.x > LEVEL_WIDTH + camera.x)
				{
					(*alien)->Receive(new Package(HIT));
				}
				// special case if camera going over right edge of level
				else if (	camera.x + WINDOW_WIDTH > LEVEL_WIDTH
						&&	(*alien)->position.x < WINDOW_WIDTH - (LEVEL_WIDTH - camera.x))
				{
					(*alien)->Receive(new Package(HIT));
				}
			}
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
		mmbHandler->Destroy();
		explosionHandler->Destroy();
		bomb_count_sprite->destroy();
	
		rockets_pool.Destroy();
		aliens_pool.Destroy();
		bombs_pool.Destroy();
		humans_pool.Destroy();

		delete aliens_grid;
		delete player;
	}
};