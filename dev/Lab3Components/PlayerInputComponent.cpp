#include "PlayerInputComponent.h"
#include "GameObject.h"
#include "GlobalVariables.h"

void PlayerInputComponent::update(AvancezLib* system, GameObject& obj)
{
	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);
	if (keys.right)
		obj.velocity = 1.0f * PLAYER_SPEED;
	else if (keys.left)
		obj.velocity = -1.0f * PLAYER_SPEED;
	else
		obj.velocity = 0;
	
	if (keys.fire)
		SDL_Log("Fireeeeee!");
}