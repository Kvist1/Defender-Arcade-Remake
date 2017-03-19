
// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 
	int bombs;

	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		lives = NUM_LIVES;
		bombs = NUM_PLAYER_BOMBS;
		facingDirection = left;
	}

	virtual void Receive(Package *p)
	{
		if (p->msg == HIT)
		{
			SDL_Log("Player::Hit!");
			RemoveLife();

			if (lives < 1)
				Send(new Package(GAME_OVER));
		}
		else if (p->msg == HIT_POWER_UP)
		{
			Send(new Package(START_POWER_UP, &this->position));
		}
	}

	void DropBomb()
	{
		if (bombs > 0) {
			bombs--;
			Send(new Package(PLAYER_BOMB_DROPPED));
		}
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}
};


class PlayerBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Rocket> * rockets_pool;
	float time_bomb_pressed;	// time from the last time the bomb button was pressed

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Rocket> * rockets_pool)
	{
		Component::Create(system, go, game_objects);
		this->rockets_pool = rockets_pool;
	}

	virtual void Init()
	{
		go->position = glm::vec2(0, WINDOW_HEIGHT / 2);

		time_fire_pressed = -10000.f;
	}

	virtual void Update(float dt, int camX, int camY)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		
		if (keys.right)
			MoveHorizontal(dt * PLAYER_SPEED);
		else if (keys.left)
			MoveHorizontal(-dt * PLAYER_SPEED);
		if (keys.up)
			MoveVertical(-dt * PLAYER_SPEED/2);
		else if (keys.down)
			MoveVertical(dt * PLAYER_SPEED/2);
		
		if (keys.fire && dt != 0)
		{
			if (CanFire())
			{
				// fetches a rocket from the pool and use it in game_objects
				Rocket * rocket = rockets_pool->FirstAvailable();
				if (rocket != NULL)	// rocket is NULL if the object pool can not provide an object
				{
					rocket->Init(go->position.x, go->position.y, go->facingDirection);
					game_objects->insert(rocket);
				}
			}
		}

		if (keys.dropBomb)
		{
			if (CanDropBomb())
			{
				Player * player = (Player *)go;
				player->DropBomb();
			}
		}
	}

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void MoveHorizontal(float move)
	{
		if (move > 0) 
			go->facingDirection = GameObject::FacingDirection::right;
		else if (move < 0)
			go->facingDirection = GameObject::FacingDirection::left;

		go->position.x += move;

		if (go->position.x > LEVEL_WIDTH)
			go->position.x = 0;
		else if (go->position.x < 0)
			go->position.x = LEVEL_WIDTH;
	}

	// move the player up or down
	// param move depends on the time, so the player moves always at the same speed on any computer
	void MoveVertical(float move)
	{
		go->position.y += move;

		if (go->position.y >= (LEVEL_HEIGHT - go->size.y))
			go->position.y = LEVEL_HEIGHT - go->size.y;

		if (go->position.y < MINIMAP_HEIGHT)
			go->position.y = MINIMAP_HEIGHT;
	}

	// return true if enough time has passed from the previous rocket
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / GAME_SPEED))
			return false;

		time_fire_pressed = system->getElapsedTime();

		SDL_Log("fire!");
		return true;
	}

	bool CanDropBomb()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_bomb_pressed) < (PLAYER_BOMB_TIME_INTERVAL / GAME_SPEED))
			return false;

		time_bomb_pressed = system->getElapsedTime();

		SDL_Log("Player::bomb!");
		return true;
	}
};
