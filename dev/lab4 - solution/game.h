
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

	Sprite * bg_sprite;
	//The camera area
	SDL_Rect camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	int bgScrollingOffset = 0;

	//Box2D
	b2World * b2_world;
	b2Body* body;
	//Box2D timestep stuff
	float32 UPDATE_INTERVAL = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	float32 MAX_CYCLES_PER_FRAME = 5;
	float32 timeAccumulator = 0;
	//Box2D test sprite
	Sprite* box2d_sprite;

public:

	virtual void Create(AvancezLib* system)
	{
		SDL_Log("Game::Create");

		this->system = system;

		// Box2D TEST #####################################################################
		box2d_sprite = system->createSprite("data/player_left.bmp");
		//create box2D world 

		// Define the gravity vector.
		b2Vec2 gravity(0.0f, 9.8f);

		// Construct a world object, which will hold and simulate the rigid bodies.
		b2_world = new b2World(gravity);

		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, 480.0f);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = b2_world->CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(1000.0f, 13.0f);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);

		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(50.0f, 4.0f);
		body = b2_world->CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(16.0f, 16.0f);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// bouncy
		fixtureDef.restitution = 0.6f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		// Box2D TEST #####################################################################

		//create background
		bg_sprite = system->createSprite("data/test_background.bmp");

		//create entities
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(system, player, &game_objects, &rockets_pool);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(system, player, &game_objects, "data/player_left.bmp", "data/player_right.bmp");
		CollideComponent * player_bomb_collision = new CollideComponent();
		player_bomb_collision->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&bombs_pool);
		CollideComponent * player_alien_collision = new CollideComponent();
		player_alien_collision->Create(system, player, &game_objects, (ObjectPool<GameObject>*)&aliens_pool);

		player->Create(b2_world);
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
			(*rocket)->Create(b2_world);
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
		aliens_grid->Create(b2_world);
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
			(*alien)->Create(b2_world);
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

			(*bomb)->Create(b2_world);
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
		UpdateBox2dWorld(dt);

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

	virtual void UpdateBox2dWorld(float dt)
	{
		//*
		timeAccumulator += dt;
		if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL))
			timeAccumulator = UPDATE_INTERVAL;

		while (timeAccumulator >= UPDATE_INTERVAL)
		{
			timeAccumulator -= UPDATE_INTERVAL;
			// Instruct the world to perform a single step of simulation.
			b2_world->Step(UPDATE_INTERVAL, velocityIterations, positionIterations);
		}
		//*/

		//b2_world->Step(UPDATE_INTERVAL, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

		if (position.y > 640)
			SDL_Log("ground doesn't work?");

		box2d_sprite->draw((int)position.x - camera.x, (int)position.y);
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
		sprintf(msg, "%07d", Score());
		system->drawText(300, 32, msg);
		sprintf(msg, "bonus: %.1fX", game_speed);
		system->drawText(510, 32, msg);

		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i*36+20, 16);

		if (IsGameOver())
		{
			sprintf(msg, "*** G A M E  O V E R ***");
			system->drawText(250, 8, msg);
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