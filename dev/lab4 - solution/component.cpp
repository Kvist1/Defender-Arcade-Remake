#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "GlobalVariables.h"
#include <typeinfo>

class Player;

void Component::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->system = system;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(system, go, game_objects);

	sprite = system->createSprite(sprite_name);
}

void RenderComponent::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name, const char * sprite2_name)
{
	Component::Create(system, go, game_objects);

	sprite = system->createSprite(sprite_name);
	sprite2 = system->createSprite(sprite2_name);
}

void RenderComponent::Update(float dt, int camX, int camY)
{
	if (!go->enabled)
		return;

	if (sprite && go->facingDirection == GameObject::FacingDirection::left) 
	{
		sprite->draw(int(go->horizontalPosition - camX), int(go->verticalPosition - camY));

		if (go->horizontalPosition < WINDOW_WIDTH && go->horizontalPosition > 0)
		{
			sprite->draw(int(go->horizontalPosition - camX + LEVEL_WIDTH), int(go->verticalPosition - camY));
		} 
		else if (go->horizontalPosition < LEVEL_WIDTH && go->horizontalPosition > LEVEL_WIDTH - WINDOW_WIDTH)
		{
			sprite->draw(int(go->horizontalPosition - camX - LEVEL_WIDTH), int(go->verticalPosition - camY));
		}
	}
	else if (sprite2 && go->facingDirection == GameObject::FacingDirection::right)
	{
		sprite2->draw(int(go->horizontalPosition - camX), int(go->verticalPosition - camY));
		
		if (typeid(*go) != typeid(Player)) {
			if (go->horizontalPosition < WINDOW_WIDTH && go->horizontalPosition > 0)
			{
				sprite->draw(int(go->horizontalPosition - camX + LEVEL_WIDTH), int(go->verticalPosition - camY));
			}
			else if (go->horizontalPosition < LEVEL_WIDTH && go->horizontalPosition > LEVEL_WIDTH - WINDOW_WIDTH)
			{
				sprite->draw(int(go->horizontalPosition - camX - LEVEL_WIDTH), int(go->verticalPosition - camY));
			}
		}
	}
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
	if (sprite2 != NULL)
		sprite2->destroy();
	sprite2 = NULL;
}


void CollideComponent::Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(system, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt, int camX, int camY)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 10) &&
				(go0->horizontalPosition < go->horizontalPosition + 10) &&
				(go0->verticalPosition   > go->verticalPosition - 10) &&
				(go0->verticalPosition   < go->verticalPosition + 10))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}
