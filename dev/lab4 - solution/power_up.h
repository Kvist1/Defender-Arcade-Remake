
enum PowerUpState
{
	inAir,
	pickedByPlayer
};


class PowerUp : public GameObject
{
public:

	virtual void Init()
	{
		GameObject::Init();
		this->position.x = 200;
		this->position.y = 200;
		this->enabled = true;
	}

	virtual void Receive(Package *m)
	{
		if (!enabled)
			return;

		if (m->msg == HIT_POWER_UP)
		{
			SDL_Log("POWERUP::Hit");
		}
	}
};

class PowerUpBehaviourComponent : public Component
{
	float timeAccumulator;
	float timeBetweenPowerUps;

public:
	virtual ~PowerUpBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects)
	{
		Component::Create(system, go, game_objects);

	}

	virtual void Init()
	{
		timeAccumulator = 0.f;	
		timeBetweenPowerUps = 10.0f;
	}

	virtual void Update(float dt, int camX, int camY)
	{
		/*if (timeForPowerUp(dt))
			SDL_Log("POWER_UP SPAWN");*/
	}

	// return true if enough time has passed from the previous power up
	bool timeForPowerUp(float dt)
	{
		timeAccumulator += dt;
		if (timeAccumulator > timeBetweenPowerUps)
		{
			timeAccumulator = 0;
			return true;
		}
		return false;
	}
};