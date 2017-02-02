#pragma once
#include "avancezlib.h"
class Player;

class GraphicsComponent
{
public:
	void update(Player& player, Graphics& graphics);

private:
	// TODO: find some way to create sprites from here, factory?

	//Sprite spriteHover_;
	//Sprite spriteFlyLeft_;
	//Sprite spriteFlyRight_;
};