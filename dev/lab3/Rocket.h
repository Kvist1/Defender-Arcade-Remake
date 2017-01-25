#pragma once
#include "Entity.h"
#include "avancezlib.h"

class Rocket : 
	public Entity
{
public:
	Rocket() : isVisible_(false), Entity(0, 0, 0) {}

	void update(int playerPosX, AvancezLib::KeyStatus key, int delta);
	void setIsVisible(bool state);
	bool getIsVisible();

private:
	bool isVisible_;
};

