
enum HumanState
{
	walking,
	abduction,
	falling
};

class Human : public GameObject
{

public:
	int direction;
	HumanState humanState;
	glm::vec2 * alienPosition;

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

	virtual void Receive(Package *m)
	{
		if (!enabled)
			return;

		if (m->msg == HIT)
		{
			enabled = false;
			Send(new Package(HUMAN_HIT)); // re-broadcast the message to signal that the human has been hit (will be used to decrease the score)
			SDL_Log("Human::Hit");
		}

		if (m->msg == ABDUCTION)
		{
			Send(new Package(GAME_ABDUCTION));
			SDL_Log("abduction beam start");
			humanState = HumanState::abduction;
			if (m->position != NULL)
				this->alienPosition = m->position;
		}

		if (m->msg == HUMAN_LOST_IN_SPACE)
		{
			Send(new Package(HUMAN_LOST_IN_SPACE));
			humanState = walking; // reset
			enabled = false;
		}

		if (m->msg == HUMAN_FALLING)
		{
			humanState = falling;
			Send(new Package(HUMAN_FALLING));
		}
	}

	void ChangeDirection()
	{
		direction *= -1;
		position.y += 32;

		if (position.y > (480 - 32))
			Send(new Package(GAME_OVER));
	}

};


class HumanBehaviourComponent : public Component
{

public:
	virtual ~HumanBehaviourComponent() {}

	virtual void Update(float dt, int camX, int camY)
	{

		Human * human = (Human *)go;

		if (human->humanState == walking)
			human->position.x += human->direction * HUMAN_SPEED * dt; // direction * speed * time

		else if (human->humanState == abduction)
		{
			human->position.y = human->alienPosition->y + 50;
			human->position.x = human->alienPosition->x + 32;
		}
		else if (human->humanState == falling)
		{
			//TODO
			SDL_Log("human faalling");
			human->position.y += 300 * dt;
		}

		if (human->position.x > LEVEL_WIDTH)
			human->position.x = 0;
		else if (human->position.x < 0)
			human->position.x = LEVEL_WIDTH;

		if (human->position.y > LEVEL_HEIGHT - 50)
		{
			human->position.y = LEVEL_HEIGHT - 50;
			human->humanState = walking;
		}
	}
};
