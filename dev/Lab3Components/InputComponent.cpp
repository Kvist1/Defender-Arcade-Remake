#include "InputComponent.h"
#include "Player.h"
#include "GlobalVariables.h"

void InputComponent::update(AvancezLib* system, Player& player)
{
	AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);
	if (keys.right)
		player.velocity = 1.0f * PLAYER_SPEED;
	else if (keys.left)
		player.velocity = -1.0f * PLAYER_SPEED;
	else
		player.velocity = 0;
	
	if (keys.fire)
		SDL_Log("Fireeeeee!");
}