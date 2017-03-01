#include <set>
#include "object_pool.h"

class GameObject;
class AvancezLib;
class Sprite;
class b2World;

class Component
{
protected:
	AvancezLib * system;
	GameObject * go;	// the game object this component is part of
	std::set<GameObject*> * game_objects;	// the global container of game objects
	b2World * b2_world; // box2d world provides coordinates for entities with box2d Bodies

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, b2World * b2_world);

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

	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name, b2World * b2_world);
	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name, const char * sprite2_name, b2World * b2_world);
	virtual void Update(float dt, int camX, int camY);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
	Sprite * GetSprite2() { return sprite2; }
};


class CollideComponent : public Component
{
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* system, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects, b2World * b2_world);
	virtual void Update(float dt, int camX, int camY);
};

