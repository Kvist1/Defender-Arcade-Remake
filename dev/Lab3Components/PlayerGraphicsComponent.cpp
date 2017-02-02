#include "PlayerGraphicsComponent.h"
#include "GameObject.h"

void PlayerGraphicsComponent::update(GameObject& obj, Graphics& graphics)
{

	//Sprite* sprite = &spriteHover_;
	Sprite* sprite = graphics.createImageSprite("data/player.bmp");
	if (obj.velocity < 0)
	{
		//sprite = &spriteFlyLeft_;
		sprite = graphics.createImageSprite("data/player.bmp");
	}
	else if (obj.velocity > 0)
	{
		//sprite = &spriteFlyRight_;
		sprite = graphics.createImageSprite("data/player.bmp");
	}

	graphics.drawSprite(*sprite, obj.horizontalPosition, obj.verticalPosition);
}