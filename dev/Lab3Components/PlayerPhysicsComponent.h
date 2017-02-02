#pragma once
#include "World.h"
#include "avancezlib.h"
#include "PhysicsComponent.h"
class GameObject;

class PlayerPhysicsComponent : public PhysicsComponent
{
public:
	virtual void update(AvancezLib* system, GameObject& obj, World& world);

private:
	SDL_Rect boundingBox_;
	float lastTime;
	float newTime;
	float dt;
};