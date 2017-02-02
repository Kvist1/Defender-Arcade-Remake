#pragma once
class GameObject;

class GraphicsComponent
{
public:
	virtual ~GraphicsComponent() {}
	virtual void update(GameObject& obj, Graphics& graphics) = 0;
};