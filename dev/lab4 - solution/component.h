#include <set>
#include "object_pool.h"

class GameObject;
class AvancezLib;
class Sprite;

class Component
{
protected:
	AvancezLib * system;
	GameObject * go;	// the game object this component is part of
	std::set<GameObject*> * game_objects;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects);

	virtual void Init() {}
	virtual void Update(float dt, int camX, int camY) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
	Sprite* sprite; 
	Sprite* sprite2;

public:

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name);
	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name, const char * sprite2_name);
	virtual void Update(float dt, int camX, int camY);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
	Sprite * GetSprite2() { return sprite2; }
};

class MiniMapRenderComponent : public Component
{
	Sprite* sprite;

	int scaling;
	int mCamX;
	int mWindowWidth;

	int halfSpriteSize; // the sprites are around 32px
	int mXPos;
	int mYPos;


public:
	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name);
	virtual void Update(float dt, int camX, int camY);
	virtual void Destroy();
};

class CollideComponent : public Component
{
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects);
	virtual void Update(float dt, int camX, int camY);
};

