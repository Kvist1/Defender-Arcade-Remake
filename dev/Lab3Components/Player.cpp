#include "GlobalVariables.h"
#include "Player.h"

void Player::Init(AvancezLib* system)
{
	this->system_ = system;
	sprite = system->createSprite("data/player.bmp");
	horizontalPosition = 320;
	verticalPosition = 480 - 32;

	//rockets.Allocate(MAX_NUM_ROCKETS);
	rocket_sprite = system->createSprite("data/rocket.bmp");
	time_fire_pressed = -10000.f;

	lives = NUM_LIVES;
	score = 0;
	game_over = false;
}


void Player::Update(World& world)
{
	/*AvancezLib::KeyStatus keys;
	system->getKeyStatus(keys);
	if (keys.right)
	Move(dt * PLAYER_SPEED);
	else if (keys.left)
	Move(-dt * PLAYER_SPEED);
	else if (keys.fire)
	Fire();*/
	input_.update(system_, *this);
	physics_.update(system_, *this, world);

	//rockets.Update(dt);

	Draw();
}

// move the player left or right
// param move depends on the time, so the player moves always at the same speed on any computer
void Player::Move(float move)
{
	horizontalPosition += move;

	if (horizontalPosition > (640 - 32))
		horizontalPosition = 640 - 32;

	if (horizontalPosition < 0)
		horizontalPosition = 0;
}

// fire a rocket if enough time has passed from the previous one
void Player::Fire()
{
	// shoot just if enough time passed by
	if ((system_->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
		return;

	// use the first rocket available
	SDL_Log("Fire!!");
	/*Rocket * rocket = (Rocket *)rockets.GetFirstAvailableElem();
	rocket->Init(horizontalPosition, 480 - 52);
	time_fire_pressed = system->getElapsedTime();*/
}


void Player::Draw()
{
	sprite->draw(int(horizontalPosition), verticalPosition);

	//rockets.Draw(rocket_sprite);

	// draw lives on the upper left side of the screen
	for (auto i = 0; i < lives; i++)
		sprite->draw(32 * i, 24);
}

void Player::RemoveLife()
{
	lives--;
	if (lives < 0)
		game_over = true;
}

void Player::Destroy()
{
	sprite->destroy();
	rocket_sprite->destroy();

	//rockets.Deallocate();
}

