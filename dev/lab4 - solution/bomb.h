#include "glm\glm.hpp"

class BombBehaviourComponent : public Component
{
public:
	virtual void Update(float dt, int camX, int camY)
	{
		go->position.y += BOMB_SPEED * dt;

		if (go->position.y > 480) // When a bomb reaches the bottom of the screen, it disappears.
			go->enabled = false;
	}
};





class Bomb : public GameObject
{
public:
	glm::vec2 direction;

	virtual void Init(float xPos, float yPos)
	{
		SDL_Log("Bomb::Init");
		GameObject::Init();

		position.x = xPos;
		position.y = yPos;
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

