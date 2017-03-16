
class RocketBehaviourComponent : public Component
{
private:
	bool passedLevelWidthRight = false;
	bool passedLevelWidthLeft = false;

public:

	void Update(float dt, int camX, int camY)
	{
		if (go->facingDirection == GameObject::FacingDirection::right)
			go->position.x += ROCKET_SPEED * dt; // rocket_speed * time
		else
			go->position.x -= ROCKET_SPEED * dt; // rocket_speed * time

		if (go->position.x > LEVEL_WIDTH)
		{
			go->position.x = 0;
			passedLevelWidthRight = true;
		}
		else if (go->position.x < 0)
		{
			go->position.x = LEVEL_WIDTH;
			passedLevelWidthLeft = true;
		}

		// When the rocket reaches a bit behind the sides of the screen, it disappears.
		if ( (go->position.x < camX - WINDOW_WIDTH || go->position.x > camX + WINDOW_WIDTH * 2)
			&& !passedLevelWidthLeft
			&& !passedLevelWidthRight) 
		{
			go->enabled = false;
			SDL_Log("ROCKET DISABLED");
		} 
		else if (passedLevelWidthRight) // special case if firing a rocket and it goes through the end of the level
		{
			if (go->position.x > WINDOW_WIDTH)
			{
				go->enabled = false;
				SDL_Log("ROCKET DISABLED");
				passedLevelWidthRight = false;
				//SDL_Log("Rocket xPos = %d", (int)go->horizontalPosition);
			}
		}
		else if (passedLevelWidthLeft)
		{
			if (go->position.x < LEVEL_WIDTH - WINDOW_WIDTH)
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

	virtual void Init(float xPos, float yPos, GameObject::FacingDirection fDirection)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		position.x = xPos;
		position.y = yPos;
		facingDirection = fDirection;
	}

	virtual void Receive(Package *m)
	{
		if (!enabled)
			return;

		if (m->msg == HIT)
		{
			enabled = false;
			SDL_Log("Rocket::Hit");
		}
	}
};