#include <ctime>

class AliensGridBehaviourComponent : public Component
{
	float timeAccumulator;
	Player *player;

	ObjectPool<Alien> * aliens_pool;
	ObjectPool<Bomb> * bombs_pool;
	ObjectPool<Human> * human_pool;

	int abductionCount; // number of abductions going on by different aliens
	int abductionsAllowed; // allowed at the same time

public:
	virtual ~AliensGridBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, Player * player, std::set<GameObject*> * game_objects, ObjectPool<Alien> * aliens_pool, ObjectPool<Bomb> * bombs_pool, ObjectPool<Human> * human_pool)
	{
		Component::Create(system, go, game_objects);

		this->aliens_pool = aliens_pool;
		this->bombs_pool = bombs_pool;
		this->player = player;
		this->human_pool = human_pool;
	}

	virtual void Init()
	{
		timeAccumulator = 0.f;	// time until next abduction try by aliens
		abductionCount = 0;
		abductionsAllowed = 1; // could be increased on a harder level

		float random_xPos;
		float random_yPos;
		srand(time(NULL));

		for (int i = 0; i < 1; i++)
		{
			random_xPos = rand() % (LEVEL_WIDTH/10);
			//generates random number  MINIMAP_HEIGHT <= yPos <= LEVEL_HEIGHT-32
			random_yPos = rand() % (LEVEL_HEIGHT-MINIMAP_HEIGHT-32) + MINIMAP_HEIGHT;

			Alien * alien = aliens_pool->FirstAvailable();
			alien->Init(random_xPos*10, random_yPos, player, bombs_pool, human_pool, &abductionCount);
			game_objects->insert(alien);
		}
	}

	virtual void Update(float dt, int camX, int camY)
	{
		// check is one of tha aliens wants to change direction becase it reached the edge of the window 
		/*if (change_direction)
			for (auto alien = aliens_pool->pool.begin(); alien != aliens_pool->pool.end(); alien++)
				if ((*alien)->enabled)
					(*alien)->ChangeDirection();*/

		// is enough time passed from the last bomb, shoot another bomb from a random active alien
		// dt = 0 means that game has been paused
		/*if (dt != 0 && CanFire())
		{
			Bomb * bomb = bombs_pool->FirstAvailable();
			if (bomb != NULL)
			{
				Alien * alien = aliens_pool->SelectRandom();
				if (alien != NULL)
				{
					bomb->Init(alien->position.x, alien->position.y + 32);
					game_objects->insert(bomb);
				}
			}
		}*/

		if (abductionCount < abductionsAllowed && IsAbductionTime(dt))
		{
			Alien * alien = aliens_pool->SelectRandom();
			alien->GoPickUpHuman();
			abductionCount++;
		}
	}

	// return true if enough time has passed from the previous abduction
	bool IsAbductionTime(float dt)
	{
		timeAccumulator += dt;
		if (timeAccumulator > 11)
		{
			timeAccumulator = 0;
			return true;
		}
		return false;
	}
};



class AliensGrid : public GameObject
{
public:

	virtual ~AliensGrid() { SDL_Log("AliensGrid::~AliensGrid"); }



	virtual void Init()
	{
		SDL_Log("AliensGrid::Init");
		GameObject::Init();
	}


};