class Bomb : public Projectile
{

public:

	void Update(float dt)
	{
		verticalPosition += BOMB_SPEED * dt;

		if (verticalPosition > 480) // When a bomb reaches the bottom of the screen, it disappears.
			Shutdown();
	}
};

