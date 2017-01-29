#pragma once
#include "avancezlib.h"

class Entity
{
protected:
	Entity(AvancezLib& system, float x, float y, float vel);
	AvancezLib& getSystem();

public:
	virtual void update();
	float x();
	float y();
	float vel();
	void setX(float x);
	void setY(float y);
	void setVel(float vel);

private:
	float x_;
	float y_;
	float vel_;
	AvancezLib* system_;
};