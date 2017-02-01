#pragma once
extern const unsigned int	NUM_LIVES;
extern const unsigned int	MAX_NUM_ROCKETS;
extern const unsigned int	MAX_NUM_BOMBS;
extern const unsigned int	POINTS_PER_ALIEN;
extern const float			FIRE_TIME_INTERVAL;
extern const float			BOMB_TIME_INTERVAL;
extern const float			PLAYER_SPEED;
extern const float			ROCKET_SPEED;
extern const float			ALIEN_SPEED;
extern const float			BOMB_SPEED;
extern const float			MAX_FPS;

extern bool game_over;
// speed of the game; it is increased each time all the aliens are hit
// it is also the score multiplier
extern float game_speed;