#pragma once

class Entity
{

public:
	Entity(float x, float y, float vel);
	
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
};