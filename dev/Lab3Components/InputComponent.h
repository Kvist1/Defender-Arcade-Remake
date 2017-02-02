#pragma once
#include "avancezlib.h"
class GameObject;

class InputComponent
{
public:
	virtual ~InputComponent() {}
	virtual void update(AvancezLib* system, GameObject& obj) = 0;
};