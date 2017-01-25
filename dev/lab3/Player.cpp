#include "Player.h"
#include "SDL.h"
#include "avancezlib.h"

void Player::update(AvancezLib::KeyStatus key, int delta)
{
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
