
#include "common.h" 

#include "MemoryPool.h"
#include "Vector2Int.h" 
#include "Direction.h"
#include "NumberAction.h"
#include "Grid.h"
#include "Simulation.h" 
#include "Node.h"

Node::Node(NodeType type, Grid * grid)
{
	this->type = type;
	this->grid = grid;
}

NumberAction* Node::getAction(Number * number)
{
	return nullptr; 
}

NodeMinus::NodeMinus(Grid * grid) : Node::Node(NodeType::MINUS, grid)
{

}

NumberAction* NodeMinus::getAction(Number * number)
{
	Vector2Int down = this->position + Vector2Int::fromDirection(DOWN);
	Vector2Int up = this->position + Vector2Int::fromDirection(UP);
	Number* numberUp = this->grid->getNumberAtPosition(&up); 
	if (number->position == up && number->direction == DOWN && this->grid->getNumberAtPosition(&this->position) == NULL && this->grid->getNumberAtPosition(&down) != NULL) {
		return new NumberActionMove(number, number->position, this->position);
	}
	else if (number->position == down && number->direction == UP && this->grid->getNumberAtPosition(&this->position) == NULL && this->grid->getNumberAtPosition(&up) != NULL) {
		return new NumberActionMergeMinus(number, numberUp); 
	}

	return new NumberActionMoveDelete(number, number->position, this->position); 
}

NodeCustom::NodeCustom(Grid * grid, Grid * subGrid) : Node::Node(NodeType::CUSTOM, grid)
{
	this->subGrid = subGrid;
}

NodeCustom::~NodeCustom()
{

}

NumberAction * NodeCustom::getAction(Number * number)
{
	bool canExecute = true; 
	std::vector<Node*> nodesInDirection = this->subGrid->inputNodes[number->direction]; 
	for (std::vector<Node*>::iterator it = nodesInDirection.begin(); it != nodesInDirection.end(); ++it) {
		NodeInput* inputNode = (NodeInput*)(*it); 
		Vector2Int forward = inputNode->position + Vector2Int::fromDirection(inputNode->direction); 
		if (inputNode->grid->getNumberAtPosition(&forward) != nullptr) {
			canExecute = false; 
		}
	}

	if (canExecute) {
		for (std::vector<Node*>::iterator it = nodesInDirection.begin(); it != nodesInDirection.end(); ++it) {
			NodeInput* inputNode = (NodeInput*)(*it);
			Vector2Int forward = inputNode->position + Vector2Int::fromDirection(inputNode->direction);
			canExecute = true;
			Number* subNumber = this->subGrid->sim->numberPool.newElement(this->subGrid);
			subNumber->position = forward;
			subNumber->direction = inputNode->direction;
			subNumber->currentAction = new NumberActionSpawnMove(subNumber, subNumber->position, forward);
			subNumber->grid = inputNode->grid;
			subNumber->value = number->value; 
			subNumber->grid->sim->addNumber(subNumber); 
		}

		return new NumberActionMoveDelete(number, number->position, this->position); 
	}
	
	return nullptr; 
}

NodeSmallerThan::NodeSmallerThan(Grid * grid) : Node::Node(NodeType::SMALLERTHAN, grid)
{

}

NumberAction * NodeSmallerThan::getAction(Number * number)
{
	if (number->position == this->position) { // on top ( move out ) 
		return new NumberActionMove(number, number->position, this->position+Vector2Int::fromDirection(number->direction)); 
	}
	else { // before enter ( delete if >= 0 else move into  ) 
		if (number->value < 0) {
			return new NumberActionMove(number, number->position, this->position);
		}
		else {
			return new NumberActionMoveDelete(number, number->position, this->position);
		}
	}
	
}

NodeInput::NodeInput(Grid * grid) : Node::Node(NodeType::NODE_INPUT, grid)
{

}

NumberAction * NodeInput::getAction(Number * number)
{
	return new NumberActionMoveDelete(number, number->position, this->position); 
}

NodeOutput::NodeOutput(Grid * grid, NodeCustom * outputNode) : Node::Node(NodeType::OUTPUT, grid)
{
	this->parentNode = outputNode;
}

NumberAction * NodeOutput::getAction(Number * number)
{
	Vector2Int forward = this->parentNode->position + Vector2Int::fromDirection(number->direction);
	if (this->parentNode->grid->getNumberAtPosition(&forward) == nullptr) {
		Number* outputNumber = this->parentNode->grid->sim->numberPool.newElement(this->parentNode->grid); 
		outputNumber->position = forward;
		outputNumber->direction = number->direction;
		outputNumber->currentAction = new NumberActionSpawnMove(outputNumber, outputNumber->position, forward);
		outputNumber->grid = this->parentNode->grid;
		outputNumber->value = number->value;
		outputNumber->grid->sim->addNumber(outputNumber);

		return new NumberActionMoveDelete(number, number->position, this->position);
	}
	else {
		return nullptr; 
	}
}

NodeGameInput::NodeGameInput(Grid * grid) : Node::Node(NodeType::GAMEINPUT, grid)
{
	
}

NumberAction * NodeGameInput::getAction(Number * number)
{
	return new NumberActionMoveDelete(number, number->position, this->position);
}

NodeGameOutput::NodeGameOutput(Grid * grid) : Node::Node(NodeType::GAMEOUTPUT, grid)
{

}

NumberAction * NodeGameOutput::getAction(Number * number)
{
	number->grid->sim->outputs.push_back(number->value); 
	return new NumberActionMoveDelete(number, number->position, this->position);
}
