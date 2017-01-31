class Alien : public Projectile
{

public:

	void Update(float dt) {}

	// move the alien left (direction = -1) or right (direction = 1)
	// return true if the alien hits the edge of the screen, false otherwise
	bool Update(float direction, float dt)
	{
		horizontalPosition += direction * ALIEN_SPEED * dt; // direction * speed * time

		if ((direction == 1) && (horizontalPosition > (640 - 32)))
			return true;
		if ((direction == -1) && (horizontalPosition < 0))
			return true;

		return false;
	}

	// move the alien down
	// return true if the alien is below the bottom edge (game over condition)
	void GoDown()
	{
		verticalPosition += 32;

		if (verticalPosition > (480 - 32))
			game_over = true;
	}
};