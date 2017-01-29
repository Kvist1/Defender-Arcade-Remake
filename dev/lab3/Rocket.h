#pragma once
#include "Entity.h"
#include "avancezlib.h"
#include "Player.h"

class Rocket : 
	public Entity
{
public:
	Rocket(AvancezLib& system, Player& player, float x, float y, float vel);

	void update();
	void setIsVisible(bool state);
	bool getIsVisible();

private:
	bool isVisible_;
	Player* player_;
};

