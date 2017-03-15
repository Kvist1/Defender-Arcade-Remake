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


	virtual void Receive(MessageNew *m) 
	{
		if (m->msg == HIT)
		{ 
			enabled = false;
			SDL_Log("Bomb::Hit");
		}
	}
};


class BombBehaviourComponent : public Component
{
	float distance; // distance flying towards target (player)

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

		if (bomb->position.x > LEVEL_WIDTH)
		{
			bomb->startPosition.x -= LEVEL_WIDTH; // new imaginary starting point
		}
		else if (bomb->position.x < 0)
		{
			bomb->startPosition.x += LEVEL_WIDTH; // new imaginary starting point
		}

		// When a bomb reaches a certain distance from starting point, it disappears.
		if (bomb->position.y < MINIMAP_HEIGHT || bomb->position.y > LEVEL_HEIGHT)
		{
			go->enabled = false;
			SDL_Log("BOMB DISABLED");
		}
	}
};
