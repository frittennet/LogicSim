#pragma once

#include "Vector2Int.h"
#include "Direction.h"

class Grid; 
class NumberAction; 

class Number
{
private: 
	static int nextId; 
public:
	NumberAction* currentAction; 
	NumberAction* lastAction; 
	Direction direction; 
	Vector2Int position; 
	Grid* grid; 
	bool deleteNextTick; 
	int value; 
	int id; 

	Number(Grid* grid);
	~Number(); 


	void tick(); 

};

