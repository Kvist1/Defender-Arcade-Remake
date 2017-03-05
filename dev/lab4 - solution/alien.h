
class Alien : public GameObject
{

public:
	int xDirection, yDirection;
	Player * player;
	ObjectPool<Bomb> * bombs_pool;

	virtual void Init(bool * change_direction, float xPos, float yPos, Player * player, ObjectPool<Bomb> * bombs_pool)
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
		position.y += 32;

		if (position.y > (480 - 32))
			Send(GAME_OVER);
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

public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Init()
	{
		time_bomb_launched = -10000.f;	// time from the last dropped bomb
		randomTime = rand() % 6; // random time between 0 - 5s 
		alienMove = GetRandomMovement();
		timeAccumulator = 0;
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

		if (go->position.y >(LEVEL_HEIGHT - 30))
			go->position.y = 482 - 32;
		else if (go->position.y < MINIMAP_HEIGHT)
			go->position.y = MINIMAP_HEIGHT;

		if (PlayerInRange() && CanFire())
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
		if (bomb != NULL)
		{
			// direction = (starting point - target point) / ||distance||
			float distance = glm::distance(alien->position, alien->player->position);
			glm::vec2 direction = (alien->player->position - alien->position) / distance;
			bomb->Init(alien->position, direction, distance);
			game_objects->insert(bomb);
		}
	}

	bool PlayerInRange()
	{
		Alien * alien = (Alien *)go;
		float distance = glm::distance(alien->position, alien->player->position);

		if (distance <= ALIEN_RANGE)
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
