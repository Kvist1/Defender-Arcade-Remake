#pragma once
#include "Entity.h"
#include "avancezlib.h"

class Player : public Entity
{
public:
	Player(AvancezLib& system, float x, float y, float vel);

	void update();
	void setIsDead(bool state);
	void moveLeft(int delta);
	void moveRight(int delta);
	void fire(int delta);

private:
	bool isDead_;
};

