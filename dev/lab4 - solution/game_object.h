// GameObject represents objects which moves are drawn
#include <vector>
#include "glm\glm.hpp"

enum Message { HIT, ALIEN_HIT, HUMAN_HIT, PLAYER_BOMB_DROPPED, GAME_OVER, LEVEL_WIN, NO_MSG };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	glm::vec2 position;
	// size = width and height
	glm::vec2 size; 
	bool enabled;
	enum FacingDirection
	{
		left,
		right
	};
	FacingDirection facingDirection;

	virtual ~GameObject();

	virtual void Create();
	virtual void Create(glm::vec2 size); // for objects that need e.g. collision checks
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt, int camX, int camY);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};