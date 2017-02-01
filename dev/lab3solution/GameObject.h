#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "World.h"
#include "Graphics.h"

class GameObject
{
public:
	int velocity;
	int x, y;

	GameObject(InputComponent* input,
		PhysicsComponent* physics,
		GraphicsComponent* graphics)
		: input_(input),
		physics_(physics),
		graphics_(graphics)
	{}

	void update(World& world, Graphics& graphics)
	{
		input_->update(*this);
		physics_->update(*this, world);
		graphics_->update(*this, graphics);
	}

private:
	InputComponent* input_;
	PhysicsComponent* physics_;
	GraphicsComponent* graphics_;
};