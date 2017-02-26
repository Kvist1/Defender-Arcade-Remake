

class Human : public GameObject
{

public:
	int direction;
	bool * change_direction;

	virtual void Init()
	{
		// bool * change_direction, double xPos, double yPos

		SDL_Log("Human::Init");
		
		int random_xPos = rand() % LEVEL_WIDTH; 
		int random_direction = rand() % 2;

		this->horizontalPosition = random_xPos;
		this->verticalPosition = 480 - 32;
		//this->change_direction = change_direction;

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
		verticalPosition += 32;

		if (verticalPosition > (480 - 32))
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

		human->horizontalPosition += human->direction * HUMAN_SPEED * dt; // direction * speed * time

		if (human->horizontalPosition > LEVEL_WIDTH)
			human->horizontalPosition = 0;
		else if (human->horizontalPosition < 0)
			human->horizontalPosition = LEVEL_WIDTH;

		/*if ((human->direction == 1) && (human->horizontalPosition > (LEVEL_WIDTH - 32)))
			*(human->change_direction) = true;

		if ((human->direction == -1) && (human->horizontalPosition < 0))
			*(human->change_direction) = true;*/
	}
};
