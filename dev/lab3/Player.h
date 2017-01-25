#pragma once
#include "Entity.h"
#include "avancezlib.h"

class Player : 
	public Entity
{
public:
	Player() : isDead_(false), Entity(0, 0, 0) {}

	void update(AvancezLib::KeyStatus key, int delta);
	void setIsDead(bool state);
	void moveLeft(int delta);
	void moveRight(int delta);
	void fire(int delta);

private:
	bool isDead_;
};

