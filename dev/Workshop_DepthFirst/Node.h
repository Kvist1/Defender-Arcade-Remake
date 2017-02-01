#pragma once

class Node
{
public:
	Node(int connection1, int connection2)
	{
		connections[0] = connection1;
		connections[1] = connection2;
	}

private:
	bool visited = false;
	int * connections = new int[2];
};