#pragma once
class Sprite;
class AvancezLib;
class Explosion;
#include <set>


class ExplosionRenderHandler
{
	AvancezLib *system;
	std::set<Explosion*> explosions_set;

public:
	virtual ~ExplosionRenderHandler();

	void Create(AvancezLib * system);

	void Update(int camX, float dt);

	void AddExplosion(float x, float y);

	void RemoveExplosion(Explosion *explosion);

	void Destroy();
};

class Explosion
{
	Sprite **sprites;
	AvancezLib *system;
	ExplosionRenderHandler *explosionHandler;
	float timeAccumulator;
	float x, y;
	int spriteNbr;

private:
	bool TimeToChangeSprite(float dt);

public:
	bool animationDone;

	virtual ~Explosion();

	void Create(AvancezLib * system, ExplosionRenderHandler *explosionHandler, float x, float y);

	void DrawExplosion(int camX, float dt);

	void Destroy();
};