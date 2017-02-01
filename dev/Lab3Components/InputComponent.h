#pragma once
#include "avancezlib.h"
class Player;

class InputComponent
{
public:
	void update(AvancezLib* system, Player& player);

private:
	const float PLAYER_SPEED = 160.0f;
};