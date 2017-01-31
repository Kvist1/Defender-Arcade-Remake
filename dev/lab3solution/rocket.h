
// rockets are shot by the player towards the aliens
class Rocket : public Projectile
{

public:

	void Update(float dt)
	{
		verticalPosition -= ROCKET_SPEED * dt; // rocket_speed * time

		if (verticalPosition < 0) // When the rocket reaches the top of the screen, it disappears.
			Shutdown();
	}
};