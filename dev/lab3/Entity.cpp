#include "Entity.h"

Entity::Entity(AvancezLib& system, float x, float y, float vel)
{
	x_ = x;
	y_ = y;
	vel_ = vel;
	system_ = &system;
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

AvancezLib& Entity::getSystem()
{
	return *system_;
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

