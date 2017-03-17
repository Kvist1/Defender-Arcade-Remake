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
		sprite->draw(int(go->position.x - camX), int(go->position.y - camY));

		/* special cases when position is close to level edge.. need to see what's in the beginning
		of the level if you are at the end and vice versa (since it loops) */
		if (go->position.x < WINDOW_WIDTH && go->position.x > 0)
		{
			sprite->draw(int(go->position.x - camX + LEVEL_WIDTH), int(go->position.y - camY));
		} 
		else if (go->position.x < LEVEL_WIDTH && go->position.x > LEVEL_WIDTH - WINDOW_WIDTH)
		{
			sprite->draw(int(go->position.x - camX - LEVEL_WIDTH), int(go->position.y - camY));
		}
	}
	else if (sprite2 && go->facingDirection == GameObject::FacingDirection::right)
	{
		sprite2->draw(int(go->position.x - camX), int(go->position.y - camY));
		
		if (typeid(*go) != typeid(Player)) {
			if (go->position.x < WINDOW_WIDTH && go->position.x > 0)
			{
				sprite->draw(int(go->position.x - camX + LEVEL_WIDTH), int(go->position.y - camY));
			}
			else if (go->position.x < LEVEL_WIDTH && go->position.x > LEVEL_WIDTH - WINDOW_WIDTH)
			{
				sprite->draw(int(go->position.x - camX - LEVEL_WIDTH), int(go->position.y - camY));
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


void MiniMapRenderComponent::Create(AvancezLib * system, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(system, go, game_objects);

	sprite = system->createSprite(sprite_name);

	scaling = LEVEL_WIDTH / MINIMAP_WIDTH;
	mWindowWidth = WINDOW_WIDTH / scaling;
	halfSpriteSize = go->size.x / 2;
	newOrigo = WINDOW_WIDTH / 2 - MINIMAP_WIDTH / 2;
}

void MiniMapRenderComponent::Update(float dt, int camX, int camY)
{
	mCamX = camX / scaling;
	
	mXPos = WINDOW_WIDTH / 2 - mWindowWidth / 2 + go->position.x / scaling - mCamX + halfSpriteSize / scaling;
	mYPos = go->position.y / scaling + halfSpriteSize / scaling;

	/* special cases when position is close to level edge.. (since it loops) */
	if (mXPos > newOrigo + MINIMAP_WIDTH)
	{
		sprite->draw(mXPos - MINIMAP_WIDTH, mYPos);
	}
	else if (mXPos < newOrigo)
	{
		sprite->draw(mXPos + MINIMAP_WIDTH, mYPos);
	} 
	else 
		sprite->draw(mXPos, mYPos);
}

void MiniMapRenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
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
			// depending on the size of the game object, check the hit box that should be from the middle of the object.
			// box should be from center of the object +- 20px in horizontal and +- 30px in vertical
			if ((go0->position.x + go0->size.x > go->position.x + go->size.x - 20) &&
				(go0->position.x + go0->size.x < go->position.x + go->size.x + 20) &&
				(go0->position.y + go0->size.y > go->position.y + go->size.y - 30) &&
				(go0->position.y + go0->size.y < go->position.y + go->size.y + 30))
			{
				go->Receive(new Package(HIT));
				go0->Receive(new Package(HIT));
			}
		}
	}
}

void PowerUpCollideComponent::Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(system, go, game_objects);
	this->coll_objects = coll_objects;
}

void PowerUpCollideComponent::Update(float dt, int camX, int camY)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			// depending on the size of the game object, check the hit box that should be from the middle of the object.
			// box should be from center of the object +- 10px in horizontal and +- 30px in vertical
			if ((go0->position.x + go0->size.x > go->position.x + go->size.x - 10) &&
				(go0->position.x + go0->size.x < go->position.x + go->size.x + 10) &&
				(go0->position.y + go0->size.y > go->position.y + go->size.y - 30) &&
				(go0->position.y + go0->size.y < go->position.y + go->size.y + 30))
			{
				go->Receive(new Package(HIT_POWER_UP));
				go0->Receive(new Package(HIT_POWER_UP));
			}
		}
	}
}


