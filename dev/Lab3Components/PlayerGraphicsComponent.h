#pragma once
#include "avancezlib.h"
#include "GraphicsComponent.h"
class GameObject;

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
	virtual void update(GameObject& obj, Graphics& graphics);

private:
	// TODO: find some way to create sprites from here, factory?

	//Sprite spriteHover_;
	//Sprite spriteFlyLeft_;
	//Sprite spriteFlyRight_;
};