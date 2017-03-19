#include <ctime>

class AliensGrid : public GameObject
{
public:
	bool resetTimer;

	virtual ~AliensGrid() { SDL_Log("AliensGrid::~AliensGrid"); }

	virtual void Init()
	{
		SDL_Log("AliensGrid::Init");
		GameObject::Init();
		resetTimer = false;
	}

	virtual void Receive(Package *m)
	{
		if (!enabled)
			return;

		if (m->msg == HUMAN_LOST_IN_SPACE || m->msg == HUMAN_FALLING)
		{
			resetTimer = true;
		}
	}
};

class AliensGridBehaviourComponent : public Component
{
	float coolTimeAbduction;

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
		this->coolTimeAbduction = 3.0f;
	}

	virtual void Init()
	{
		timeAccumulator = 0.f;	// time counting against next abduction try by aliens
		abductionCount = 0;
		abductionsAllowed = 1; // could be increased on a harder wave

		float random_xPos;
		float random_yPos;
		srand(time(NULL));

		for (int i = 0; i < 10; i++)
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
		AliensGrid * alienGrid = (AliensGrid *)go;
		if (alienGrid->resetTimer) {
			timeAccumulator = 0;
			coolTimeAbduction = getRandomTime();
			alienGrid->resetTimer = false;
		}

		// controls if humans should to be picked up
		if (abductionCount < abductionsAllowed && IsAbductionTime(dt))
		{
			Alien * alien = aliens_pool->SelectRandom();
			alien->GoPickUpHuman();
		}
	}

	int getRandomTime()
	{
		return (rand() % 7) + 3; // random number between 3-10
	}

	// return true if enough time has passed from the previous abduction
	bool IsAbductionTime(float dt)
	{
		timeAccumulator += dt;
		if (timeAccumulator > coolTimeAbduction)
		{
			timeAccumulator = 0;
			return true;
		}
		return false;
	}
};
