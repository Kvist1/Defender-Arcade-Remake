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

	// test
	/*Explosion *e1 = new Explosion();
	e1->Create(system, this, 100, 100);
	explosions_set.insert(e1);

	Explosion *e2 = new Explosion();
	e2->Create(system, this, 100, 200);
	explosions_set.insert(e2);

	Explosion *e3 = new Explosion();
	e3->Create(system, this, 100, 300);
	explosions_set.insert(e3);*/
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
	if (timeAccumulator > 0.18f)
	{
		timeAccumulator = 0;
		return true;
	}
	return false;
}

void Explosion::DrawExplosion(int camX, float dt)
{
	sprites[spriteNbr]->draw(x - camX, y);
	if (TimeToChangeSprite(dt))
		spriteNbr++; //(spriteNbr++) % 6;
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
