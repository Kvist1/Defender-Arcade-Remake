#pragma once
#include "GameObject.h"

class PhysicsComponent
{
public:
	virtual ~PhysicsComponent() {}
	virtual void update(GameObject& obj, World& world) = 0;
};