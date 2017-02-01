#pragma once
#include "avancezlib.h"

class World
{

public:
	World(AvancezLib& system)
	{
		system_ = system;
	}

	void update(float dt)
	{

	}

	float getDeltaTime()
	{
		
	}

private:
	AvancezLib system_;
};