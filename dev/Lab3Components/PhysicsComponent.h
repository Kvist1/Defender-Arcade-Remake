#pragma once
#include "World.h"
class GameObject;

class PhysicsComponent
{
public:
	virtual ~PhysicsComponent() {}
	virtual void update(AvancezLib* system, GameObject& obj, World& world) = 0;
};
