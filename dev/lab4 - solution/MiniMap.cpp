#include "MiniMap.h"
#include "avancezlib.h"
#include "GlobalVariables.h"

MiniMapBackgroundHandler::~MiniMapBackgroundHandler()
{
	SDL_Log("MiniMapBackgroundHandler::~MiniMapBackgroundHandler");
}

void MiniMapBackgroundHandler::Create(AvancezLib * system, const char * sprite_name)
{
	this->system = system;
	this->background_sprite = system->createSprite(sprite_name);
}

void MiniMapBackgroundHandler::DrawMiniMap(int camX)
{
	int scaling = LEVEL_WIDTH / MINIMAP_WIDTH;
	int mCamX = camX / scaling;
	int mWindowWidth = WINDOW_WIDTH / scaling;

	/*
	creates 3 continous copies of the minimap (level), to simulate the loop/warp
		 ____________________________________
	<-	|   left    |   middle   |   right   |  ->
		|___________|____________|___________|
						|	|
					camera width

	moves left and right depending on camera x position
	*/
	int mini_map_x_position = WINDOW_WIDTH / 2 - mWindowWidth / 2 - mCamX;
	background_sprite->draw(mini_map_x_position - MINIMAP_WIDTH, 0);
	background_sprite->draw(mini_map_x_position, 0);
	background_sprite->draw(mini_map_x_position + MINIMAP_WIDTH, 0);
}

void MiniMapBackgroundHandler::DrawMiniMapCameraBox()
{
	int scaling = LEVEL_WIDTH / MINIMAP_WIDTH;

	// creates the two boxes cowering the "extra level backgrounds"
	SDL_Rect rect = { 0, 0, WINDOW_WIDTH / 4, MINIMAP_HEIGHT };
	SDL_Rect rect2 = { 3 * WINDOW_WIDTH / 4, 0, WINDOW_WIDTH / 4, MINIMAP_HEIGHT };
	SDL_Rect line = { 0, MINIMAP_HEIGHT, WINDOW_WIDTH, 5 };

	// creates a visual of the camera width in the minimap, quite messy but is just drawing  rectangles in correct positions
	SDL_Rect camera_left_edge = { WINDOW_WIDTH / 2 - WINDOW_WIDTH / scaling / 2, 5, 2, MINIMAP_HEIGHT - 10 };
	SDL_Rect camera_left_edge_top_wing = { WINDOW_WIDTH / 2 - WINDOW_WIDTH / scaling / 2, 5, 10, 2 };
	SDL_Rect camera_left_edge_bottom_wing = { WINDOW_WIDTH / 2 - WINDOW_WIDTH / scaling / 2, MINIMAP_HEIGHT - 5, 10, 2 };

	SDL_Rect camera_right_edge = { WINDOW_WIDTH / 2 + WINDOW_WIDTH / scaling / 2, 5, 2, MINIMAP_HEIGHT - 10 };
	SDL_Rect camera_right_edge_top_wing = { WINDOW_WIDTH / 2 + WINDOW_WIDTH / scaling / 2 - 10, 5, 10, 2 };
	SDL_Rect camera_right_edge_bottom_wing = { WINDOW_WIDTH / 2 + WINDOW_WIDTH / scaling / 2 - 8, MINIMAP_HEIGHT - 5, 10, 2 };

	// Render them
	SDL_SetRenderDrawColor(system->renderer, 20, 20, 20, 255);
	SDL_RenderFillRect(system->renderer, &rect);
	SDL_RenderFillRect(system->renderer, &rect2);
	SDL_RenderFillRect(system->renderer, &line);
	SDL_SetRenderDrawColor(system->renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(system->renderer, &camera_left_edge);
	SDL_RenderFillRect(system->renderer, &camera_left_edge_top_wing);
	SDL_RenderFillRect(system->renderer, &camera_left_edge_bottom_wing);
	SDL_RenderFillRect(system->renderer, &camera_right_edge);
	SDL_RenderFillRect(system->renderer, &camera_right_edge_top_wing);
	SDL_RenderFillRect(system->renderer, &camera_right_edge_bottom_wing);
	SDL_SetRenderDrawColor(system->renderer, 255, 255, 255, 255);
}

void MiniMapBackgroundHandler::Destroy()
{
	background_sprite->destroy();
}
