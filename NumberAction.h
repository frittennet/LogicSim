#pragma once

#include "Vector2Int.h" 
#include "LogicSim.h" 

class Number; 

class NumberAction
{
public:
	Number* number; 
	bool wasExecuted = false; 
	APINumberAction type; 

	NumberAction(Number* number);
	~NumberAction(); 

	virtual bool execute(Number* caller);
};

class NumberActionMove : public NumberAction {
public:
	Vector2Int from; 
	Vector2Int to; 
	NumberActionMove(Number* number, Vector2Int from, Vector2Int to); 

	bool execute(Number* caller);
};

class NumberActionMoveDelete : public NumberActionMove {
public:
	NumberActionMoveDelete(Number* number, Vector2Int from, Vector2Int to);

	bool execute(Number* caller);
};

class NumberActionMergeMinus : public NumberActionMoveDelete {
public: 
	Number* other; 
	
	NumberActionMergeMinus(Number* number, Number* other);

	bool execute(Number* caller);

}; 

class NumberActionComposite : public NumberAction {
public: 
	NumberAction* actions; 
	int num_actions; 

	NumberActionComposite(Number* number, NumberAction* actions, int num_actions); 
	~NumberActionComposite(); 

	bool execute(Number* caller);
};

class NumberActionSpawnMove : public NumberActionMove {
public:
	NumberActionSpawnMove(Number* number, Vector2Int from, Vector2Int to); 

	bool execute(Number* caller);
};