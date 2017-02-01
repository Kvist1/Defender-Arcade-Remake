#pragma once
#include "GameObject.h"
#include "avancezlib.h"

class InputComponent
{
public:
	virtual ~InputComponent() {}
	virtual void update(AvancezLib& system, GameObject& gameObject) = 0;
};

class PlayerInputComponent : public InputComponent
{
public:
	virtual void update(AvancezLib& system, GameObject& player)
	{
		AvancezLib::KeyStatus keys;
		system.getKeyStatus(keys);
		if (keys.left)
			player.x -= PLAYER_SPEED;
		else if (keys.right)
			player.x += PLAYER_SPEED;
		else if (keys.fire)
			SDL_Log("Fire!");
	}

private:
	const float PLAYER_SPEED = 160.0f;
};