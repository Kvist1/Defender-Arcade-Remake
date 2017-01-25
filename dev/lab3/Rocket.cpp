#include "Rocket.h"

void Rocket::update(int playerPosX, AvancezLib::KeyStatus key, int delta)
{
	if (isVisible_) 
	{
		SDL_Log("rocket!\n");
		setY(y() - vel() * delta);
		if (y() < -32)
		{
			setIsVisible(false);
		}
	} 
	else if (!isVisible_ && key.fire)
	{
		setX(playerPosX+16);
		setY(415);
		setIsVisible(true);
		setY(y() - vel() * delta);
	}

}

void Rocket::setIsVisible(bool state)
{
	isVisible_ = state;
}

