#include "Number.h" 
#include "NumberAction.h"
#include "Grid.h"
#include "LogicSim.h" 

NumberAction::NumberAction(Number* number)
{
	printf("NumberAction Constructor \n"); 

	type = APINumberAction::ACTION_NONE; 
	this->number = number; 
	this->type = type; 
}

NumberAction::~NumberAction()
{
	printf("NumberAction Deconstructor \n"); 

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
	NumberAction::execute(caller);
	Number* blockingNumber = this->number->grid->getNumberAtPosition(&to);
	bool blocked = blockingNumber != nullptr && blockingNumber != this->number; 
	if (blocked) {
		blocked = !blockingNumber->currentAction->execute(caller);
	}
	if (!blocked) {
		NumberAction::execute(caller);
		this->number->grid->removeNumber(this->number);
		this->number->position = this->number->position + Vector2Int::forDirection[this->number->direction];
		this->number->grid->setNumber(this->number);
		printf("Move [%i, %i] => [%i, %i] \n", from.x, from.y, to.x, to.y); 
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
	NumberAction::execute(caller);
	this->number->grid->removeNumber(this->number); 
	return true; 
}

NumberActionSpawnMove::NumberActionSpawnMove(Number* number, Vector2Int from, Vector2Int to) : NumberActionMove::NumberActionMove(number, from, to) 
{
	type = APINumberAction::ACTION_SPAWN; 
}

bool NumberActionSpawnMove::execute(Number* caller) 
{
	return NumberActionMove::execute(caller); 
}

NumberActionComposite::NumberActionComposite(Number* number, NumberAction* actions, int num_actions) : NumberAction::NumberAction(number) 
{
	this->actions = actions; 
	this->num_actions = num_actions; 
}

NumberActionComposite::~NumberActionComposite()
{
	delete[] this->actions; 
}

bool NumberActionComposite::execute(Number* caller) 
{
	NumberAction::execute(caller); 
	for (int i = 0; i < this->num_actions; i++) {
		if (!this->actions[i].execute(caller)) {
			return false;
		}
	}
	return true;
}

NumberActionMergeMinus::NumberActionMergeMinus(Number * number, Number * other) : NumberActionMoveDelete::NumberActionMoveDelete(number, number->position, other->position)
{
	APINumberAction::ACTION_MERGE; 
	this->other = other; 
}

bool NumberActionMergeMinus::execute(Number* caller)
{
	other->value = other->value - number->value; 
	return NumberActionMoveDelete::execute(caller); 
}
