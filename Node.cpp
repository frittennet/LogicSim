
#include "common.h" 

#include "Vector2Int.h" 
#include "Direction.h"
#include "NumberAction.h"
#include "Grid.h"
#include "Node.h"

Node::Node(NodeType type, Grid * grid)
{
	this->type = type;
	this->grid = grid;
}

NumberAction * Node::getAction(Number * number)
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

	return nullptr; 
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
	return nullptr;
}

NodeSmallerThan::NodeSmallerThan(Grid * grid) : Node::Node(NodeType::SMALLERTHAN, grid)
{

}

NumberAction * NodeSmallerThan::getAction(Number * number)
{
	return nullptr;
}

NodeInput::NodeInput(Grid * grid) : Node::Node(NodeType::NODE_INPUT, grid)
{

}

NumberAction * NodeInput::getAction(Number * number)
{
	return nullptr;
}

NodeOutput::NodeOutput(Grid * grid, NodeCustom * outputNode) : Node::Node(NodeType::OUTPUT, grid)
{
	this->parentNode = outputNode;
}

NumberAction * NodeOutput::getAction(Number * number)
{
	return nullptr;
}

NodeGameInput::NodeGameInput(Grid * grid) : Node::Node(NodeType::GAMEINPUT, grid)
{

}

NumberAction * NodeGameInput::getAction(Number * number)
{
	return nullptr;
}

NodeGameOutput::NodeGameOutput(Grid * grid) : Node::Node(NodeType::GAMEOUTPUT, grid)
{

}

NumberAction * NodeGameOutput::getAction(Number * number)
{
	return nullptr;
}
