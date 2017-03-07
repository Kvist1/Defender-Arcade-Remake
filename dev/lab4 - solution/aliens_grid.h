#include <ctime>

class AliensGridBehaviourComponent : public Component
{
	float time_bomb_launched;
	Player *player;

	ObjectPool<Alien> * aliens_pool;
	ObjectPool<Bomb> * bombs_pool;

public:
	virtual ~AliensGridBehaviourComponent() {}

	virtual void Create(AvancezLib* system, GameObject * go, Player * player, std::set<GameObject*> * game_objects, ObjectPool<Alien> * aliens_pool, ObjectPool<Bomb> * bombs_pool)
	{
		Component::Create(system, go, game_objects);

		this->aliens_pool = aliens_pool;
		this->bombs_pool = bombs_pool;
		this->player = player;
	}

	virtual void Init()
	{
		time_bomb_launched = -10000.f;	// time fromthe last time a bomb was dropped by one of the aliens

		float random_xPos;
		float random_yPos;
		srand(time(NULL));

		for (int i = 0; i < 10; i++)
		{
			random_xPos = rand() % (LEVEL_WIDTH/10);
			//generates random number  MINIMAP_HEIGHT <= yPos <= LEVEL_HEIGHT-32
			random_yPos = rand() % (LEVEL_HEIGHT-MINIMAP_HEIGHT-32) + MINIMAP_HEIGHT;

			Alien * alien = aliens_pool->FirstAvailable();
			alien->Init(random_xPos*10, random_yPos, player, bombs_pool);
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
	}

	// return true if enough time has passed from the previous bomb
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_bomb_launched) < (BOMB_TIME_INTERVAL / GAME_SPEED))
			return false;

		// drop the bomb with 3% chance
		if ((rand() / (float)RAND_MAX) < 0.97f)
			return false;

		time_bomb_launched = system->getElapsedTime();

		SDL_Log("AlienGrid::bomb!");
		return true;
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