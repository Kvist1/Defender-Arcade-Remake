#include "Rocket.h"

Rocket::Rocket(AvancezLib& system, Player& player, float x, float y, float vel)
	: Entity(system, x, y, vel)
{
	isVisible_ = false;
	player_ = &player;
}

void Rocket::update()
{
	AvancezLib::KeyStatus key;
	AvancezLib system = getSystem();
	system.getKeyStatus(key);
	int delta = system.getDelta();

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
		setX((*player_).x()+16);
		setY(415);
		setIsVisible(true);
		setY(y() - vel() * delta);
	}

}

void Rocket::setIsVisible(bool state)
{
	isVisible_ = state;
}

bool Rocket::getIsVisible()
{
	return isVisible_;
}

