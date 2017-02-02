#ifndef __AVANCEZ_LIB__
#define __AVANCEZ_LIB__

#include "SDL.h"
#include "SDL_ttf.h"

class Sprite
{
	SDL_Texture * texture;

public:

	Sprite(SDL_Texture * texture);

	// Destroys the sprite instance
	void destroy();

	// Draw the sprite at the given position.
	// Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
	// (All sprites are 32*32 pixels, clipping is not supported)
	void draw(SDL_Renderer * renderer, int x, int y);
};

class Graphics
{
	SDL_Renderer * renderer;

public:
	Graphics(SDL_Renderer * renderer);

	bool init();

	void destroy();

	// Create a sprite given a string.
	// All sprites are 32*32 pixels.
	Sprite* createImageSprite(const char* name);

	void drawSprite(Sprite sprite, int x, int y);

	// Draws the given text.
	void drawText(int x, int y, const char* msg);

private:
	TTF_Font* font;
};


class AvancezLib
{
public:
	// Destroys the avancez library instance
	void destroy();

	// Creates the main window. Returns true on success.
	bool init(int width, int height);

	// Clears the screen and draws all sprites and texts which have been drawn
	// since the last update call.
	// If update returns false, the application should terminate.
	bool update();
	Graphics* createGraphics();


	// Return the total time spent in the game, in seconds.
	float getElapsedTime();

	struct KeyStatus
	{
		bool fire; // space
		bool left; // left arrow
		bool right; // right arrow
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);

private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	KeyStatus key;
};





#endif // __AVANCEZ_LIB__
