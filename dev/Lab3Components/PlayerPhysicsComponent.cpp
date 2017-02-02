#include "PlayerPhysicsComponent.h"
#include "GameObject.h"

void PlayerPhysicsComponent::update(AvancezLib* system, GameObject& obj, World & world)
{
	newTime = system->getElapsedTime();
	dt = newTime - lastTime;
	lastTime = newTime;

	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);
	if (obj.velocity != 0)
		obj.horizontalPosition += obj.velocity*dt;
	world.resolveCollision(boundingBox_, obj.horizontalPosition,
		obj.verticalPosition, obj.velocity);
}
