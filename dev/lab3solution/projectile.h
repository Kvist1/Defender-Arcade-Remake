// Projectile represents objects which moves, are drawn, do not store internal arrays and are preallocated
// rockets, bombs and aliens are inherited from this class
class Projectile
{
public:

	float horizontalPosition;
	double verticalPosition;
	bool enabled;

	Projectile()
	{
		enabled = false;
	}

	void Init(float horizontalPosition, float verticalPosition)
	{
		this->horizontalPosition = horizontalPosition;
		this->verticalPosition = verticalPosition;
		enabled = true;
	}

	virtual void Update(float dt) = 0;

	void Draw(Sprite* sprite)
	{
		sprite->draw(int(horizontalPosition), int(verticalPosition));
	}

	void Shutdown()
	{
		enabled = false;
	}
};