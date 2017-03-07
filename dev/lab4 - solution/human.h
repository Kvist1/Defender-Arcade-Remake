
enum HumanState
{
	walking,
	abduction
};

class Human : public GameObject
{

public:
	int direction;
	HumanState humanState;

	virtual void Init()
	{
		SDL_Log("Human::Init");

		int random_xPos = rand() % LEVEL_WIDTH; 
		int random_direction = rand() % 2;

		this->position.x = random_xPos;
		this->position.y = LEVEL_HEIGHT - 50;

		if (random_direction == 0)
			direction = -1;
		else
			direction = 1;

		enabled = true;
		humanState = HumanState::walking;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			Send(HUMAN_HIT); // re-broadcast the message to signal that the human has been hit (will be used to decrease the score)
			SDL_Log("Human::Hit");
		}

		if (m == ABDUCTION)
		{
			Send(HUMAN_ABDUCTION);
			SDL_Log("abduction beam start");
			humanState = HumanState::abduction;
		}
	}

	void ChangeDirection()
	{
		direction *= -1;
		position.y += 32;

		if (position.y > (480 - 32))
			Send(GAME_OVER);
	}

};


class HumanBehaviourComponent : public Component
{

public:
	virtual ~HumanBehaviourComponent() {}

	virtual void Update(float dt, int camX, int camY)
	{

		Human * human = (Human *)go;

		if (human->humanState == HumanState::walking)
			human->position.x += human->direction * HUMAN_SPEED * dt; // direction * speed * time

		else if (human->humanState == HumanState::abduction)
			human->position.x = human->position.x; // TODO!!!!

		if (human->position.x > LEVEL_WIDTH)
			human->position.x = 0;
		else if (human->position.x < 0)
			human->position.x = LEVEL_WIDTH;
	}
};
