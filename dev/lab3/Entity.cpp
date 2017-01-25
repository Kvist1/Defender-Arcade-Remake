#include "Entity.h"

Entity::Entity(float x, float y, float vel)
{
	x_ = x;
	y_ = y;
	vel_ = vel;
}

void Entity::update()
{
}

float Entity::x()
{
	return x_;
}

float Entity::y()
{
	return y_;
}

float Entity::vel()
{
	return vel_;
}

void Entity::setX(float x)
{
	x_ = x;
}

void Entity::setY(float y)
{
	y_ = y;
}

void Entity::setVel(float vel)
{
	vel_ = vel;
}

