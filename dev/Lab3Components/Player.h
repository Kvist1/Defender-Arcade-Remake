#pragma once
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "World.h"

// the main player
class Player
{
	AvancezLib* system_;

	float time_fire_pressed;	// time from the last time the fire button was pressed

public:
	float horizontalPosition;
	float verticalPosition;
	float velocity;
	int score;
	int lives;	// it's game over when goes below zero 

	void Init(AvancezLib* system);

	void Update(World& world, Graphics& graphics);

	// fire a rocket if enough time has passed from the previous one
	void Fire();

	void RemoveLife();

	void Destroy();

private:
	InputComponent input_;
	PhysicsComponent physics_;
	GraphicsComponent graphics_;
};
