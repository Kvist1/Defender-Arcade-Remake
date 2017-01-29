#pragma once
#include "Entity.h"
#include "avancezlib.h"

class Rocket : 
	public Entity
{
public:
	Rocket(AvancezLib& system, float x, float y, float vel);

	void update(int playerPosX, AvancezLib::KeyStatus key, int delta);
	void setIsVisible(bool state);
	bool getIsVisible();

private:
	bool isVisible_;
};

