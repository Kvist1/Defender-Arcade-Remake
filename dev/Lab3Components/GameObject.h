#pragma once
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "World.h"

// the main player
class GameObject
{
	AvancezLib* system_;
public:
	float horizontalPosition;
	float verticalPosition;
	float velocity;

	GameObject(	InputComponent* input, 
			PhysicsComponent* physics, 
			GraphicsComponent* graphics) 
	: input_(input), physics_(physics), graphics_(graphics) {}

	void Init(AvancezLib* system);

	void Update(World& world, Graphics& graphics);

	void Destroy();

private:
	InputComponent* input_;
	PhysicsComponent* physics_;
	GraphicsComponent* graphics_;
};
