#include "ExplosionRenderHandler.h"
#include "avancezlib.h"
#include "GlobalVariables.h"
#include <set>

ExplosionRenderHandler::~ExplosionRenderHandler()
{
	SDL_Log("ExplosionRenderHandler::~ExplosionRenderHandler");
}

void ExplosionRenderHandler::Create(AvancezLib * system)
{
	this->system = system;
}

void ExplosionRenderHandler::Update(int camX, float dt)
{
	if (!explosions_set.empty())
	{
		for (std::set<Explosion*>::iterator it = explosions_set.begin(); it != explosions_set.end();)
		{
			(*it)->DrawExplosion(camX, dt);

			if ((*it)->animationDone)
				it = explosions_set.erase(it);
			else
				++it;
		}
	}
}

void ExplosionRenderHandler::AddExplosion(float x, float y)
{
	float adjustedX = x - 10;
	float adjustedY = y - 30; 
	Explosion *explosion = new Explosion();
	explosion->Create(system, this, adjustedX, adjustedY);
	explosions_set.insert(explosion);
}

void ExplosionRenderHandler::RemoveExplosion(Explosion * explosion)
{
	explosions_set.erase(explosion);
}


void ExplosionRenderHandler::Destroy()
{
	for (std::set<Explosion*>::iterator it = explosions_set.begin(); it != explosions_set.end();)
	{
		(*it)->Destroy();
		it = explosions_set.erase(it);
	}
}


void Explosion::Create(AvancezLib * system, ExplosionRenderHandler *explosionHandler, float x, float y)
{
	this->x = x;
	this->y = y;
	this->explosionHandler = explosionHandler;
	this->animationDone = false;

	sprites = new Sprite *[10];

	sprites[0] = system->createSprite("data/alien_explosion_0.png");
	sprites[1] = system->createSprite("data/alien_explosion_1.png");
	sprites[2] = system->createSprite("data/alien_explosion_2.png");
	sprites[3] = system->createSprite("data/alien_explosion_3.png");
	sprites[4] = system->createSprite("data/alien_explosion_4.png");
	sprites[5] = system->createSprite("data/alien_explosion_5.png");
	sprites[6] = system->createSprite("data/alien_explosion_5.png");
}

Explosion::~Explosion()
{
	SDL_Log("ExplosionRenderHandler::~ExplosionRenderHandler");
}

bool Explosion::TimeToChangeSprite(float dt)
{
	timeAccumulator += dt;
	if (timeAccumulator > 0.1f)
	{
		timeAccumulator = 0;
		return true;
	}
	return false;
}

void Explosion::DrawExplosion(int camX, float dt)
{
	sprites[spriteNbr]->draw(x - camX, y); // normal case
	
	/* special cases when position is close to level edge.. need to see what's in the beginning
	of the level if you are at the end and vice versa (since it loops) */
	if (x < WINDOW_WIDTH && x > 0)
	{
		sprites[spriteNbr]->draw(x - camX + LEVEL_WIDTH, y);
	}
	else if (x < LEVEL_WIDTH && x > LEVEL_WIDTH - WINDOW_WIDTH)
	{
		sprites[spriteNbr]->draw(x - camX - LEVEL_WIDTH, y);
	}

	if (TimeToChangeSprite(dt))
		spriteNbr++; 
	if (spriteNbr == 6)
		animationDone = true;
}

void Explosion::Destroy()
{
	// destroy sprites
	sprites[0] = NULL;
	sprites[1] = NULL;
	sprites[2] = NULL;
	sprites[3] = NULL;
	sprites[4] = NULL;
	sprites[5] = NULL;
	sprites[6] = NULL;

	sprites = NULL;
}
