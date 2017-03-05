class Player;

class Alien : public GameObject
{

public:
	int xDirection, yDirection;
	Player * player;

	virtual void Init(bool * change_direction, double xPos, double yPos, Player * player)
	{
		SDL_Log("Alien::Init");
		GameObject::Init();
		this->horizontalPosition = xPos;
		this->verticalPosition = yPos;

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
		verticalPosition += 32;

		if (verticalPosition > (480 - 32))
			Send(GAME_OVER);
	}

};


class AlienBehaviourComponent : public Component
{
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
			alien->horizontalPosition += alien->xDirection * ALIEN_SPEED * dt; // direction * speed * time
		} 
		/*else if (alienMove == AlienMove::vertical)
		{
			alien->verticalPosition += alien->yDirection * ALIEN_SPEED * dt;
		}*/
		else if (alienMove == AlienMove::diagonal)
		{
			alien->horizontalPosition += alien->xDirection * ALIEN_SPEED * dt;
			alien->verticalPosition += alien->yDirection * ALIEN_SPEED * dt;
		}

		// keep in bounds of level
		if (alien->horizontalPosition > LEVEL_WIDTH)
			alien->horizontalPosition = 0;
		else if (alien->horizontalPosition < 0)
			alien->horizontalPosition = LEVEL_WIDTH;

		if (go->verticalPosition >(LEVEL_HEIGHT - 30))
			go->verticalPosition = 482 - 32;
		else if (go->verticalPosition < MINIMAP_HEIGHT)
			go->verticalPosition = MINIMAP_HEIGHT;
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

	bool PlayerInRange()
	{
		Alien * alien = (Alien *)go;
		//alien->player->horizontalPosition;
		//alien->player->verticalPosition;

		return false;
	}
};
