#include <iostream> 

#include "common.h" 

#include "NumberAction.h"
#include "Number.h"
#include "Vector2Int.h"
#include "Grid.h"
#include "NumberAction.h" 

int Number::nextId = 0; 

Number::Number(Grid* grid) 
{
	id = nextId++; 
	this->grid = grid; 
	this->direction = Direction::UP;
	this->position = Vector2Int::zero;
	this->deleteNextTick = false; 
	this->lastAction = nullptr; 
	this->currentAction = nullptr; 
}

Number::~Number()
{
	delete lastAction; 
	delete currentAction; 
	
}

void Number::tick()
{
	debugPrint("Number Tick \n"); 

	if (currentAction != nullptr && currentAction->wasExecuted) {
		delete lastAction; 
		lastAction = currentAction;
		currentAction = nullptr;
	}
	
	if (currentAction == nullptr) {
		Vector2Int forward = position + Vector2Int::forDirection[this->direction]; 
		Node* nodeCurrent = this->grid->getNodeAtPosition(&position);
		Node* nodeForward = this->grid->getNodeAtPosition(&forward);
		if (nodeCurrent != nullptr) {
			currentAction = nodeCurrent->getAction(this);
		}
		else if (nodeForward != nullptr) {
			currentAction = nodeForward->getAction(this); 
		}
		else {
			currentAction = new NumberActionMove(this, position, forward);
		}
	}

}
