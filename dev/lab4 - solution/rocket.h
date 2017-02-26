
class RocketBehaviourComponent : public Component
{
private:
	bool passedLevelWidthRight = false;
	bool passedLevelWidthLeft = false;

public:

	void Update(float dt, int camX, int camY)
	{
		if (go->facingDirection == GameObject::FacingDirection::right)
			go->horizontalPosition += ROCKET_SPEED * dt; // rocket_speed * time
		else
			go->horizontalPosition -= ROCKET_SPEED * dt; // rocket_speed * time

		if (go->horizontalPosition > LEVEL_WIDTH)
		{
			go->horizontalPosition = 0;
			passedLevelWidthRight = true;
		}
		else if (go->horizontalPosition < 0)
		{
			go->horizontalPosition = LEVEL_WIDTH;
			passedLevelWidthLeft = true;
		}

		// When the rocket reaches a bit behind the sides of the screen, it disappears.
		if ( (go->horizontalPosition < camX - WINDOW_WIDTH || go->horizontalPosition > camX + WINDOW_WIDTH * 2)
			&& !passedLevelWidthLeft
			&& !passedLevelWidthRight) 
		{
			go->enabled = false;
			SDL_Log("ROCKET DISABLED");
		} 
		else if (passedLevelWidthRight) // special case if firing a rocket and it goes through the end of the level
		{
			if (go->horizontalPosition > WINDOW_WIDTH)
			{
				go->enabled = false;
				SDL_Log("ROCKET DISABLED");
				passedLevelWidthRight = false;
				//SDL_Log("Rocket xPos = %d", (int)go->horizontalPosition);
			}
		}
		else if (passedLevelWidthLeft)
		{
			if (go->horizontalPosition < LEVEL_WIDTH - WINDOW_WIDTH)
			{
				go->enabled = false;
				SDL_Log("ROCKET DISABLED");
				passedLevelWidthLeft = false;
				//SDL_Log("Rocket xPos = %d", (int)go->horizontalPosition);
			}
		}
	}
};


// rockets are shot by the player towards the aliens
class Rocket : public GameObject
{

public:

	virtual void Init(double xPos, double yPos, GameObject::FacingDirection fDirection)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
		facingDirection = fDirection;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Rocket::Hit");
		}
	}
};