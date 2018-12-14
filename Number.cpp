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
		currentAction = NULL;
	}
	
	if (currentAction == NULL) {
		Vector2Int forward = position + Vector2Int::forDirection[this->direction]; 
		Node* nodeCurrent = this->grid->getNodeAtPosition(&position);
		Node* nodeForward = this->grid->getNodeAtPosition(&forward);
		if (nodeCurrent != NULL) {
			currentAction = nodeCurrent->getAction(this);
		}
		else if (nodeForward != NULL) {
			currentAction = nodeForward->getAction(this); 
		}
		else {
			currentAction = new NumberActionMove(this, position, forward);
		}
	}

}
