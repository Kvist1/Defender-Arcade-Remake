#include "glm\glm.hpp"

class Bomb : public GameObject
{
public:
	glm::vec2 direction, startPosition;
	float travelDistance;

	virtual void Init(glm::vec2 position, glm::vec2 direction, float travelDistance)
	{
		SDL_Log("Bomb::Init");
		GameObject::Init();

		this->position = position;
		this->startPosition = position;
		this->direction = direction;
		this->facingDirection = FacingDirection::left;
		this->travelDistance = travelDistance;
	}


	virtual void Receive(Message m) 
	{
		if (m == HIT)
		{ 
			enabled = false;
			SDL_Log("Bomb::Hit");
		}
	}
};


class BombBehaviourComponent : public Component
{
	float distance; // distance flying towards target (player)

private:
	bool passedLevelWidthRight = false;
	bool passedLevelWidthLeft = false;

public:
	virtual void Init()
	{
		distance = 0;
	}

	virtual void Update(float dt, int camX, int camY)
	{
		Bomb * bomb = (Bomb *)go;

		distance += dt * BOMB_SPEED;
		bomb->position = bomb->startPosition + (bomb->direction * distance);

		// When a bomb reaches a certain distance from starting point, it disappears.
		if (bomb->position.y < 0 || bomb->position.y > 480)
		{
			go->enabled = false;
			SDL_Log("BOMB DISABLED");
		}
	}
};
