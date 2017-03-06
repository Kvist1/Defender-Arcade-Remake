

class Human : public GameObject
{

public:
	int direction;

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

		human->position.x += human->direction * HUMAN_SPEED * dt; // direction * speed * time

		if (human->position.x > LEVEL_WIDTH)
			human->position.x = 0;
		else if (human->position.x < 0)
			human->position.x = LEVEL_WIDTH;
	}
};
