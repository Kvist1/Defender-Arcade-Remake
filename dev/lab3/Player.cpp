#include "Player.h"
#include "SDL.h"
#include "avancezlib.h"

Player::Player(AvancezLib& system, float x, float y, float vel)
	: Entity(system, x, y, vel)
{
	isDead_ = false;
}

void Player::update(AvancezLib& system2)
{
	AvancezLib::KeyStatus key;
	AvancezLib system = getSystem();
	system.getKeyStatus(key);
	int delta = system.getDelta();

	if (key.fire) fire(delta);
	if (key.left) moveLeft(delta);
	if (key.right) moveRight(delta);
}

void Player::setIsDead(bool state)
{
	isDead_ = state;
}

void Player::moveLeft(int delta)
{
	if (x() - vel() * delta < 0)
	{
		setX(0);
	}
	else 
	{
		setX(x() - vel() * delta); 
	}
	SDL_Log("Left!\n");
}

void Player::moveRight(int delta)
{
	if (x() + vel() * delta > 640-64)
	{
		setX(640-64);
	}
	else
	{
		setX(x() + vel() * delta);
	}
	SDL_Log("Right!\n");
}

void Player::fire(int delta)
{
	SDL_Log("Fire ");
}
