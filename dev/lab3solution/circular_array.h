// implementation of a simple CircularArray for storing bombs and rockets
// in this way, they are instantiated once and used when needed
template <class T>
class CircularArray
{
	T * array;
	unsigned int size;
	unsigned int first_available;

public:

	void Allocate(unsigned int size)
	{
		this->size = size;
		array = new T[size];
		first_available = 0;
	}

	void Deallocate()
	{
		delete[] array;
	}

	// get the first element in the array which is not enabled
	T * GetFirstAvailableElem()
	{
		for (auto i = 0; i < size; i++)
		if (array[first_available].enabled)
			first_available = (first_available + 1) % size;

		return &(array[first_available]);
	}

	T * GetAt(unsigned int i)
	{
		//assert(i < size);

		return &(array[i]);
	}

	void Update(float dt)
	{
		for (auto i = 0; i < size; i++)
		if (array[i].enabled)
			array[i].Update(dt);
	}

	void Draw(Sprite* sprite)
	{
		for (auto i = 0; i < size; i++)
		if (array[i].enabled)
			array[i].Draw(sprite);
	}
};
