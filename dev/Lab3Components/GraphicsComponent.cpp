#include "GraphicsComponent.h"
#include "Player.h"

void GraphicsComponent::update(Player& player, Graphics& graphics)
{

	//Sprite* sprite = &spriteHover_;
	Sprite* sprite = graphics.createImageSprite("data/player.bmp");
	if (player.velocity < 0)
	{
		//sprite = &spriteFlyLeft_;
		sprite = graphics.createImageSprite("data/player.bmp");
	}
	else if (player.velocity > 0)
	{
		//sprite = &spriteFlyRight_;
		sprite = graphics.createImageSprite("data/player.bmp");
	}

	graphics.drawSprite(*sprite, player.horizontalPosition, player.verticalPosition);
}