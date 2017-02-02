#pragma once
#include "avancezlib.h"
#include "InputComponent.h"
class Player;

class PlayerInputComponent : public InputComponent
{	

public:
	virtual void update(AvancezLib* system, GameObject& obj);
};