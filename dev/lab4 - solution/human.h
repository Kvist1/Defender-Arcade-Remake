
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

	virtual void Receive(MessageNew *m)
	{
		if (!enabled)
			return;

		if (m->msg == HIT)
		{
			enabled = false;
			Send(new MessageNew(HUMAN_HIT)); // re-broadcast the message to signal that the human has been hit (will be used to decrease the score)
			SDL_Log("Human::Hit");
		}

		if (m->msg == ABDUCTION)
		{
			Send(new MessageNew(GAME_ABDUCTION));
			SDL_Log("abduction beam start");
			humanState = HumanState::abduction;
			if (m->position != NULL)
				this->alienPosition = m->position;
		}

		if (m->msg == HUMAN_LOST_IN_SPACE)
		{
			Send(new MessageNew(HUMAN_LOST_IN_SPACE));
			humanState = walking; // reset
			enabled = false;
		}
	}

	void ChangeDirection()
	{
		direction *= -1;
		position.y += 32;

		if (position.y > (480 - 32))
			Send(new MessageNew(GAME_OVER));
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
		{
			//TODO
			human->position.y = human->alienPosition->y + 50;
			human->position.x = human->alienPosition->x + 32;
		}

		if (human->position.x > LEVEL_WIDTH)
			human->position.x = 0;
		else if (human->position.x < 0)
			human->position.x = LEVEL_WIDTH;
	}
};
