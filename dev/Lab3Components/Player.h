#pragma once
#include "InputComponent.h"

// the main player
class Player
{
	Sprite * sprite, *rocket_sprite;
	AvancezLib* system_;

	float time_fire_pressed;	// time from the last time the fire button was pressed

public:
	int lives;	// it's game over when goes below zero 
	float horizontalPosition;
	float verticalPosition;
	float velocity;

	int score;


	void Init(AvancezLib* system);


	void Update(float dt);

	// move the player left or right, param move depends on the time, 
	// so the player moves always at the same speed on any computer
	void Move(float move);

	// fire a rocket if enough time has passed from the previous one
	void Fire();

	void Draw();

	void RemoveLife();

	void Destroy();

private:
	InputComponent input_;
};
