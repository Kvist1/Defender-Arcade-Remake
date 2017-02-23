

class PlayerBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Rocket> * rockets_pool;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Rocket> * rockets_pool)
	{
		Component::Create(system, go, game_objects);
		this->rockets_pool = rockets_pool;
	}

	virtual void Init()
	{
		go->horizontalPosition = 320;
		go->verticalPosition = 480 - 32;

		time_fire_pressed = -10000.f;
		acceleration_speed = 0.f;
	}

	float acceleration_speed;
	virtual void Update(float dt, int camX, int camY)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		if (keys.right)
			MoveHorizontal(dt * PLAYER_SPEED);
		else if (keys.left)
			MoveHorizontal(-dt * PLAYER_SPEED);
		if (keys.up)
			MoveVertical(-dt * PLAYER_SPEED/2);
		else if (keys.down)
			MoveVertical(dt * PLAYER_SPEED/2);
		if (keys.fire)
		{
			if (CanFire())
			{
				// fetches a rocket from the pool and use it in game_objects
				Rocket * rocket = rockets_pool->FirstAvailable();
				if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
				{
					rocket->Init(go->horizontalPosition, go->verticalPosition, go->facingDirection);
					game_objects->insert(rocket);
				}
			}
		}
	}

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void MoveHorizontal(float move)
	{
		if (move > 0) 
			go->facingDirection = GameObject::FacingDirection::right;
		else if (move < 0)
			go->facingDirection = GameObject::FacingDirection::left;

		go->horizontalPosition += move;

		if (go->horizontalPosition > (640 - 32))
			go->horizontalPosition = 640 - 32;
			
		if (go->horizontalPosition < 0)
			go->horizontalPosition = 0;
	}

	// move the player up or down
	// param move depends on the time, so the player moves always at the same speed on any computer
	void MoveVertical(float move)
	{
		go->verticalPosition += move;

		if (go->verticalPosition >(482 - 32))
			go->verticalPosition = 482 - 32;

		if (go->verticalPosition < 50)
			go->verticalPosition = 50;
	}

	// return true if enough time has passed from the previous rocket
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
			return false;

		time_fire_pressed = system->getElapsedTime();

		SDL_Log("fire!");
		return true;
	}
};

class PlayerSlideComponent : public Component
{

private:
	float right_speed = 0.f;
	float left_speed = 0.f;

public:

	void Update(float dt, int camX, int camY)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		if (keys.right)
		{
			//SDL_Log("Slide right");
			if (left_speed > 0)
			{
				MoveHorizontal(-dt * left_speed*2);
				left_speed -= 0.5f;
			} 
			else if (right_speed < PLAYER_SPEED)
			{
				//MoveHorizontal(dt * test_speed); // slow acceleration, add later somewhere...
				right_speed += 0.9f;
			}
		}
		else if (keys.left)
		{
			//SDL_Log("Slide left");
			if (right_speed > 0)
			{
				MoveHorizontal(dt * right_speed*2);
				right_speed -= 0.5f;
			} 
			else if (left_speed < PLAYER_SPEED)
			{
				//MoveHorizontal(-dt * test_speed); // slow acceleration, add later somewhere...
				left_speed += 0.9f;
			}
		}

		else if (right_speed > 0 && go->facingDirection == GameObject::FacingDirection::right)
		{
			MoveHorizontal(dt * right_speed);
			right_speed -= 0.1f;
		}
		else if (right_speed > 0 && go->facingDirection == GameObject::FacingDirection::left)
		{
			MoveHorizontal(dt * right_speed );
			right_speed -= 0.5f;
		}
		else if (left_speed > 0 && go->facingDirection == GameObject::FacingDirection::right)
		{
			MoveHorizontal(-dt * left_speed );
			left_speed -= 0.5f;
		}
		else if (left_speed > 0 && go->facingDirection == GameObject::FacingDirection::left)
		{
			MoveHorizontal(-dt * left_speed);
			left_speed -= 0.1f;
		}
	}

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void MoveHorizontal(float move)
	{
		/*if (move > 0)
			go->facingDirection = GameObject::FacingDirection::right;
		else if (move < 0)
			go->facingDirection = GameObject::FacingDirection::left;*/

		go->horizontalPosition += move;

		if (go->horizontalPosition >(640 - 32))
			go->horizontalPosition = 640 - 32;

		if (go->horizontalPosition < 0)
			go->horizontalPosition = 0;
	}
};

// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 

	virtual ~Player()	{		SDL_Log("Player::~Player");	}

	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		lives = NUM_LIVES;
		facingDirection = left;
	}

	virtual void Receive(Message m) 
	{
		if (m == HIT)
		{ 
			SDL_Log("Player::Hit!");
			RemoveLife();

			if (lives < 0)
				Send(GAME_OVER);
		}
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}
};
