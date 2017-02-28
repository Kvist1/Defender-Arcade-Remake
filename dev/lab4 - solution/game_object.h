// GameObject represents objects which moves are drawn
#include <vector>

#include "Box2D\Box2D.h"

enum Message { HIT, ALIEN_HIT, HUMAN_HIT, GAME_OVER, LEVEL_WIN, NO_MSG };

class Component;

class GameObject
{

protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

public:
	double horizontalPosition;
	double verticalPosition;
	bool enabled;
	enum FacingDirection
	{
		left,
		right
	};
	FacingDirection facingDirection;
	b2World* b2_world;
	b2Body* b2_body;

	virtual ~GameObject();

	virtual void Create(b2World* b2_world);
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt, int camX, int camY);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
};