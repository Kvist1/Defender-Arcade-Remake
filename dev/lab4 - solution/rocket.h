
class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt, int camX, int camY)
	{
		if (go->facingDirection == GameObject::FacingDirection::right)
			go->horizontalPosition += ROCKET_SPEED * dt; // rocket_speed * time
		else
			go->horizontalPosition -= ROCKET_SPEED * dt; // rocket_speed * time

		if (go->horizontalPosition < camX - WINDOW_WIDTH || go->horizontalPosition > camX + WINDOW_WIDTH * 2) // When the rocket reaches the sides of the screen, it disappears.
		{
			go->enabled = false;
			SDL_Log("ROCKET DISABLED");
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