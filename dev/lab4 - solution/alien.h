
class Alien : public GameObject
{

public:
	int xDirection, yDirection;
	Player * player;
	ObjectPool<Bomb> * bombs_pool;

	virtual void Init(float xPos, float yPos, Player * player, ObjectPool<Bomb> * bombs_pool)
	{
		SDL_Log("Alien::Init");
		GameObject::Init();
		this->position.x = xPos;
		this->position.y = yPos;

		xDirection = rand() % 2;
		if (xDirection == 0)
			xDirection = -1;
		else
			xDirection = 1;

		yDirection = rand() % 2;
		if (yDirection == 0)
			yDirection = -1;
		else
			yDirection = 1;

		enabled = true;

		this->bombs_pool = bombs_pool;
		this->player = player;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			Send(ALIEN_HIT); // re-broadcast the message to signal that the aliens has been hit (used to increase the score)
			SDL_Log("Alien::Hit");
		}
	}

	void ChangeDirection()
	{
		xDirection *= -1;
	}
};


class AlienBehaviourComponent : public Component
{
	float time_bomb_launched;
private:
	int randomTime; // to make the alien move uncontinous. milliseconds
	float timeAccumulator;
	enum AlienMove
	{
		horizontal,
		diagonal
	};

	AlienMove alienMove;
	bool shootThroughLevelEdgeLeft;
	bool shootThroughLevelEdgeRight;
	float distanceToPlayer;

public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Init()
	{
		time_bomb_launched = -10000.f;	// time from the last dropped bomb
		randomTime = rand() % 6; // random time between 0 - 5s 
		alienMove = GetRandomMovement();
		timeAccumulator = 0;
		shootThroughLevelEdgeRight = false; // needed to check if alien want to shoot through level edges
		shootThroughLevelEdgeLeft = false;
		distanceToPlayer = -1;
	}

	virtual void Update(float dt, int camX, int camY)
	{

		Alien * alien = (Alien *)go;

		if (TimeToChangeMovement(dt))
			alienMove = GetRandomMovement();

		if (alienMove == AlienMove::horizontal)
		{
			alien->position.x += alien->xDirection * ALIEN_SPEED * dt; // direction * speed * time
		} 
		/*else if (alienMove == AlienMove::vertical)
		{
			alien->verticalPosition += alien->yDirection * ALIEN_SPEED * dt;
		}*/
		else if (alienMove == AlienMove::diagonal)
		{
			alien->position.x += alien->xDirection * ALIEN_SPEED * dt;
			alien->position.y += alien->yDirection * ALIEN_SPEED * dt;
		}

		// keep in bounds of level
		if (alien->position.x > LEVEL_WIDTH)
			alien->position.x = 0;
		else if (alien->position.x < 0)
			alien->position.x = LEVEL_WIDTH;

		if (go->position.y >(LEVEL_HEIGHT - go->size.y - 90))
			go->position.y = LEVEL_HEIGHT - go->size.y - 90;
		else if (go->position.y < MINIMAP_HEIGHT)
			go->position.y = MINIMAP_HEIGHT;

		//SDL_Log("Alien->playerPos: %0.1f", alien->player->position.x);

		if (dt != 0 && PlayerInRange() && CanFire())
			fire(dt);
	}

	AlienMove GetRandomMovement()
	{
		return (AlienMove)(rand() % 2); // random move pattern
	}

	bool TimeToChangeMovement(float dt)
	{
		timeAccumulator += dt;
		if (timeAccumulator > randomTime)
		{
			timeAccumulator = 0;

			Alien * alien = (Alien *)go;
			alien->yDirection *= -1;
			randomTime = rand() % 5;
			return true;
		}

		return false;
	}

	void fire(float dt)
	{
		Alien * alien = (Alien *)go;

		Bomb * bomb = alien->bombs_pool->FirstAvailable();

		// direction = (target point - starting point) / ||distance||
		glm::vec2 direction;

		if (bomb != NULL)
		{
			if (shootThroughLevelEdgeRight)
			{
				// bomb starting point need to be simulated from aliens position.x
				glm::vec2 simulatedPosition(alien->position.x - LEVEL_WIDTH, alien->position.y);
				direction = (alien->player->position - simulatedPosition) / distanceToPlayer;
				shootThroughLevelEdgeRight = false;
				SDL_Log("shootThroughLevelEdgeRight: TRUE");
			}
			else if (shootThroughLevelEdgeLeft)
			{
				// bomb starting point need to be simulated from aliens position.x
				glm::vec2 simulatedPosition(alien->position.x + LEVEL_WIDTH, alien->position.y);
				direction = (alien->player->position - simulatedPosition) / distanceToPlayer;
				shootThroughLevelEdgeLeft = false;
			}
			else
			{
				direction = (alien->player->position - alien->position) / distanceToPlayer;
			}

			bomb->Init(alien->position, direction, distanceToPlayer);
			game_objects->insert(bomb);
		}
	}

	bool PlayerInRange()
	{
		Alien * alien = (Alien *)go;

		// alien close to level end and player close to level start
		if (alien->position.x + ALIEN_RANGE > LEVEL_WIDTH && alien->player->position.x - ALIEN_RANGE < 0)
		{
			glm::vec2 simulatedPosition(alien->position.x - LEVEL_WIDTH, alien->position.y);
			distanceToPlayer = glm::distance(simulatedPosition, alien->player->position);
			shootThroughLevelEdgeRight = true;
		}
		// alien close to level start and player close to level end
		else if (alien->position.x - ALIEN_RANGE < 0 && alien->player->position.x + ALIEN_RANGE > LEVEL_WIDTH)
		{
			glm::vec2 simulatedPosition(alien->position.x + LEVEL_WIDTH, alien->position.y);
			distanceToPlayer = glm::distance(simulatedPosition, alien->player->position);
			shootThroughLevelEdgeLeft = true;
		}
		else
		{
			distanceToPlayer = glm::distance(alien->position, alien->player->position);
			shootThroughLevelEdgeLeft = false;
			shootThroughLevelEdgeRight = false;
		}

		if (distanceToPlayer <= ALIEN_RANGE)
		{
			//SDL_Log("Alien:: Player in range!");
			return true;
		}

		return false;
	}

	// return true if enough time has passed from the previous bomb
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_bomb_launched) < (BOMB_TIME_INTERVAL / GAME_SPEED))
			return false;

		// drop the bomb with 3% chance
		if ((rand() / (float)RAND_MAX) < 0.97f)
			return false;

		time_bomb_launched = system->getElapsedTime();

		SDL_Log("Alien::bomb!");
		return true;
	}
};
