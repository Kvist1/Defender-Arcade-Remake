#pragma once
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "World.h"

// the main player
class Player
{
	AvancezLib* system_;

public:
	float horizontalPosition;
	float verticalPosition;
	float velocity;
	
	void Init(AvancezLib* system);

	void Update(World& world, Graphics& graphics);

	void Destroy();

private:
	InputComponent input_;
	PhysicsComponent physics_;
	GraphicsComponent graphics_;
};
