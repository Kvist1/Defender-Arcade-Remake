#pragma once

#include "GlobalVariables.h"
#include "GameObject.h"

void GameObject::Init(AvancezLib* system)
{
	this->system_ = system;
	horizontalPosition = 320;
	verticalPosition = 480 - 32;
}

void GameObject::Update(World& world, Graphics& graphics)
{

	input_->update(system_, *this);
	physics_->update(system_, *this, world);
	graphics_->update(*this, graphics);

	// draw lives on the upper left side of the screen
	//for (auto i = 0; i < lives; i++)
	//graphics.drawSprite(*graphics.createImageSprite("data/player.bmp"), 32 * i, 24);
}

void GameObject::Destroy()
{

}

