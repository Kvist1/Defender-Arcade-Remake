#pragma once
#include "GameObject.h"

class GraphicsComponent
{
public:
	virtual ~GraphicsComponent() {}
	virtual void update(GameObject& obj, Graphics& graphics) = 0;
};