#include "PhysicsComponent.h"
#include "Player.h"

void PhysicsComponent::update(AvancezLib* system, Player & player, World & world)
{
	newTime = system->getElapsedTime();
	dt = newTime - lastTime;
	lastTime = newTime;

	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);
	if (player.velocity != 0)
		player.horizontalPosition += player.velocity*dt;
	world.resolveCollision(boundingBox_, player.horizontalPosition,
		player.verticalPosition, player.velocity);
}
