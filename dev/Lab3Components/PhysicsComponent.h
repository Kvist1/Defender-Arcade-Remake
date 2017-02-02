#pragma once
#include "World.h"
#include "avancezlib.h"
class Player;

class PhysicsComponent
{
public:
	void update(AvancezLib* system, Player& player, World& world);

private:
	SDL_Rect boundingBox_;
	float lastTime;
	float currentTime;
	float dt;
};