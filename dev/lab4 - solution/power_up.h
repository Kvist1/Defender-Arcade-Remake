
enum PowerUpState
{
	inAir,
	pickedByPlayer,
	inActive
};


class PowerUp : public GameObject
{

public:
	Player * player;
	PowerUpState powerUpState;
	glm::vec2 * playerPosition;
	ObjectPool<Alien> * aliens_pool;
	ObjectPool<Bomb> * bombs_pool;

	virtual void Init(ObjectPool<Alien> * aliens_pool, ObjectPool<Bomb> * bombs_pool)
	{
		GameObject::Init();
		this->position.x = 200;
		this->position.y = -100;
		this->enabled = true;
		this->powerUpState = inActive;
		this->aliens_pool = aliens_pool;
		this->bombs_pool = bombs_pool;
	}

	virtual void Receive(Package *p)
	{
		if (!enabled)
			return;

		if (p->msg == HIT_POWER_UP)
		{
			//SDL_Log("POWERUP::Hit");
		} 
		else if (p->msg == START_POWER_UP)
		{
			powerUpState = pickedByPlayer;
			if (p->position != NULL)
				this->playerPosition = p->position;
		}
	}
};

class PowerUpBehaviourComponent : public Component
{
	float time_bomb_launched;
private:
	float timeToNextPowerUp;
	float timeAccumulator;
	float timeActive;

	bool shootThroughLevelEdgeLeft;
	bool shootThroughLevelEdgeRight;
	float distanceToAlien;
	Alien *closestAlien;


public:
	virtual ~PowerUpBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects)
	{
		Component::Create(system, go, game_objects);

	}

	virtual void Init()
	{
		time_bomb_launched = -10000.f;	// time from the last dropped bomb
		timeAccumulator = 0.0f;	
		timeToNextPowerUp = 10.0f;
		distanceToAlien = -1;
		shootThroughLevelEdgeRight = false; // needed to check if power_up want to shoot through level edges
		shootThroughLevelEdgeLeft = false;
		timeActive = 0.0f;
	}

	virtual void Update(float dt, int camX, int camY)
	{
		PowerUp * power_up = (PowerUp *)go;

		if (dt != 0)
		{
			if (power_up->powerUpState == pickedByPlayer) {
				power_up->position.x = power_up->playerPosition->x;
				power_up->position.y = power_up->playerPosition->y;

				// fire against alien if possible
				closestAlien = findClosestAlien();
				if (closestAlien != NULL && AnyAlienInRange() && CanFire())
					fireAgainst(closestAlien);

				timeActive += dt;
				if (timeActive > 10.0f)
				{
					power_up->powerUpState = inActive;
					power_up->position.y = -100;
					timeActive = 0;
					timeToNextPowerUp = (rand() % 6) + 5; // random time between 5-10s 
				}

			}
			else if (power_up->powerUpState == inActive)
			{
				if (timeForPowerUp(dt))
				{
					power_up->position.x = rand() % LEVEL_WIDTH;
					power_up->position.y = (rand() % (LEVEL_HEIGHT-MINIMAP_HEIGHT)) + MINIMAP_HEIGHT-50; // set margin where it can spawn
					power_up->powerUpState = inAir;
				}
			}
			else if (power_up->powerUpState == inAir)
			{
				//do nothing
			}
		}
	}

	void fireAgainst(Alien * alien)
	{
		PowerUp * power_up = (PowerUp *)go;

		Bomb * bomb = power_up->bombs_pool->FirstAvailable();

		// direction = (target point - starting point) / ||distance||
		glm::vec2 direction;

		if (bomb != NULL)
		{
			if (shootThroughLevelEdgeRight)
			{
				// bomb starting point need to be simulated from aliens position.x
				glm::vec2 simulatedPosition(power_up->position.x - LEVEL_WIDTH, power_up->position.y);
				direction = (closestAlien->position - simulatedPosition) / distanceToAlien;
				shootThroughLevelEdgeRight = false;
			}
			else if (shootThroughLevelEdgeLeft)
			{
				// bomb starting point need to be simulated from aliens position.x
				glm::vec2 simulatedPosition(power_up->position.x + LEVEL_WIDTH, power_up->position.y);
				direction = (closestAlien->position - simulatedPosition) / distanceToAlien;
				shootThroughLevelEdgeLeft = false;
			}
			else
			{
				direction = (closestAlien->position - power_up->position) / distanceToAlien;
			}

			float bomb_speed = 1000.0f;
			bomb->Init(power_up->position, direction, distanceToAlien, bomb_speed);
			game_objects->insert(bomb);
		}
	}

	bool AnyAlienInRange()
	{
		PowerUp * power_up = (PowerUp *)go;

		// powerup/player close to level end and alien close to level start
		if (power_up->position.x + ALIEN_RANGE > LEVEL_WIDTH && closestAlien->position.x - ALIEN_RANGE < 0)
		{
			glm::vec2 simulatedPosition(power_up->position.x - LEVEL_WIDTH, power_up->position.y);
			distanceToAlien = glm::distance(simulatedPosition, closestAlien->position);
			shootThroughLevelEdgeRight = true;
		}
		// powerup/player close to level start and alien close to level end
		else if (power_up->position.x - ALIEN_RANGE < 0 && closestAlien->position.x + ALIEN_RANGE > LEVEL_WIDTH)
		{
			glm::vec2 simulatedPosition(power_up->position.x + LEVEL_WIDTH, power_up->position.y);
			distanceToAlien = glm::distance(simulatedPosition, closestAlien->position);
			shootThroughLevelEdgeLeft = true;
		}
		else
		{
			distanceToAlien = glm::distance(power_up->position, closestAlien->position);
			shootThroughLevelEdgeLeft = false;
			shootThroughLevelEdgeRight = false;
		}

		if (distanceToAlien <= ALIEN_RANGE)
		{
			//SDL_Log("Alien:: Player in range!");
			return true;
		}

		return false;
	}

	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_bomb_launched) < (BOMB_TIME_INTERVAL / GAME_SPEED / 20))
			return false;

		// drop the bomb with 3% chance
		if ((rand() / (float)RAND_MAX) < 0.97f)
			return false;

		time_bomb_launched = system->getElapsedTime();

		SDL_Log("Alien::bomb!");
		return true;
	}

	// return true if enough time has passed from the previous power up
	bool timeForPowerUp(float dt)
	{
		timeAccumulator += dt;
		if (timeAccumulator > timeToNextPowerUp)
		{
			timeAccumulator = 0;
			return true;
		}
		return false;
	}

	Alien* findClosestAlien()
	{
		PowerUp * power_up = (PowerUp *)go;

		distanceToAlien = 10000.0f; // init value
		Alien * h = NULL;

		for (auto alien = power_up->aliens_pool->pool.begin(); alien != power_up->aliens_pool->pool.end(); alien++)
		{
			if ((*alien)->enabled)
			{
				float temp = glm::distance(power_up->position, (*alien)->position);
				if (temp < distanceToAlien)
				{
					distanceToAlien = temp;
					h = (*alien);
				}
			}
		}
		return h;
	}
};