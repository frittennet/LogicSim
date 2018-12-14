



#include "common.h" 

#include "Number.h" 
#include "NumberAction.h"
#include "Grid.h"
#include "LogicSim.h" 
#include "Simulation.h"

NumberAction::NumberAction(Number* number)
{
	debugPrint("NumberAction Constructor \n"); 

	type = APINumberAction::ACTION_NONE; 
	this->number = number; 
	this->type = type; 
}

NumberAction::~NumberAction()
{
	debugPrint("NumberAction Deconstructor \n"); 

}

bool NumberAction::execute(Number* caller)
{
	wasExecuted = true; 
	return true; 
}

NumberActionMove::NumberActionMove(Number* number, Vector2Int from, Vector2Int to) : NumberAction::NumberAction(number)
{
	type = APINumberAction::ACTION_MOVE; 
	this->from = from; 
	this->to = to; 
}

bool NumberActionMove::execute(Number* caller) 
{
	Number* blockingNumber = this->number->grid->getNumberAtPosition(&to);
	bool blocked = blockingNumber != nullptr && blockingNumber != caller && ((blockingNumber->direction + 2) % 4) != number->direction; 
	if (blocked) {
		if (blockingNumber->currentAction != nullptr) {
			blocked = !blockingNumber->currentAction->execute(caller);
		}
	}
	if (!blocked) {
		NumberAction::execute(caller);
		this->number->grid->removeNumber(this->number);
		this->number->position = this->number->position + Vector2Int::forDirection[this->number->direction];
		this->number->grid->setNumber(this->number);
		debugPrint("[%i] => Move [%i, %i] => [%i, %i] \n", this->number->id, from.x, from.y, to.x, to.y); 
		return true; 
	}
	
	return false; 
}

NumberActionMoveDelete::NumberActionMoveDelete(Number* number, Vector2Int from, Vector2Int to) : NumberActionMove::NumberActionMove(number, from, to) 
{
	type = APINumberAction::ACTION_DELETE; 
}

bool NumberActionMoveDelete::execute(Number* caller)
{
	debugPrint("[%i] => Delete [%i, %i] => [%i, %i] \n", this->number->id, from.x, from.y, to.x, to.y);
	NumberAction::execute(caller);
	this->number->deleteNextTick = true; 
	this->number->grid->removeNumber(this->number); 
	return true; 
}

NumberActionSpawnMove::NumberActionSpawnMove(Number* number, Vector2Int from, Vector2Int to) : NumberActionMove::NumberActionMove(number, from, to) 
{
	type = APINumberAction::ACTION_SPAWN; 
}

bool NumberActionSpawnMove::execute(Number* caller) 
{
	debugPrint("[%i] => Spawn [%i, %i] => [%i, %i] \n", this->number->id, from.x, from.y, to.x, to.y);
	return NumberActionMove::execute(caller); 
}

NumberActionMergeMinus::NumberActionMergeMinus(Number* number, Number* other) : NumberActionMoveDelete::NumberActionMoveDelete(number, number->position, other->position)
{
	APINumberAction::ACTION_MERGE; 
	this->other = other; 
}

bool NumberActionMergeMinus::execute(Number* caller)
{
	debugPrint("[%i] => Merge [%i, %i] => [%i, %i] \n", this->number->id, from.x, from.y, to.x, to.y);
	other->value = other->value - number->value; 
	return NumberActionMoveDelete::execute(caller); 
}
