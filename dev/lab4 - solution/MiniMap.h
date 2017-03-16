#pragma once
class Sprite;
class AvancezLib;

class MiniMapBackgroundHandler
{
	Sprite *background_sprite;
	AvancezLib *system;

public:

	virtual ~MiniMapBackgroundHandler();

	void Create(AvancezLib * system, const char * sprite_name);

	void DrawMiniMap(int camX);

	void DrawMiniMapCameraBox();

	void Destroy();
};