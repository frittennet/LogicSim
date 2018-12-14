#include <algorithm> 

#include "common.h" 

#include "NodeType.h" 
#include "Grid.h"
#include "Node.h" 

using namespace std;

Grid::Grid(Simulation* sim)
{
	this->sim = sim; 
}

Grid::~Grid()
{
	nodeAtPosition.clear(); 
	std::unordered_map<int64_t, Number*>::iterator it2 = numberAtPosition.begin();
	while (it2 != numberAtPosition.end()) {
		delete it2->second;
		it2 = numberAtPosition.erase(it2++);
	}
}

int64_t Grid::getMapIndex(Vector2Int* position) {
	return ((int64_t)position->x << 32) | position->y;
}

void Grid::setNode(Node* node)
{
	bool found = false;
	try {
		found = nodeAtPosition.at(getMapIndex(&node->position)) != NULL;
	}
	catch (...) {
		
	}
	if (!found) {
		if (node->type == NodeType::NODE_INPUT || node->type == NodeType::GAMEINPUT) {
			this->inputNodes[node->direction].push_back(node); 
		}
		nodeAtPosition.insert(std::pair<int64_t, Node*>(getMapIndex(&node->position), node));
	}
	else {
		debugPrint("Tried to insert number over existing. [%i] \n", id);
	}
}

void Grid::removeNode(Node* node)
{
	if (node->type == NodeType::NODE_INPUT || node->type == NodeType::GAMEINPUT) {
		std::vector<Node*>::iterator position = std::find(inputNodes[node->direction].begin(), inputNodes[node->direction].end(), node);
		inputNodes[node->direction].erase(position);
	}
	
	nodeAtPosition.erase(getMapIndex(&node->position));
}

Node* Grid::getNodeAtPosition(Vector2Int* position)
{
	return nodeAtPosition[getMapIndex(position)];
}

void Grid::setNumber(Number* number)
{
	bool found = false;
	try {
		found = numberAtPosition.at(getMapIndex(&number->position)) != NULL;
	}
	catch (...) {
		
	}
	if (!found) {
		numberAtPosition.insert(std::pair<int64_t, Number*>(getMapIndex(&number->position), number));
	}
	else {
		debugPrint("Tried to insert number over existing. [%i] \n", id);
	}
}

void Grid::removeNumber(Number* number)
{
	Number* numExisting = NULL; 
	try {
		numExisting = numberAtPosition.at(getMapIndex(&number->position)); 
	}
	catch (...) {

	}
	if (numExisting == number) { // check if the numbers position refers to itself ( for circular movement ) 
		numberAtPosition.erase(getMapIndex(&number->position)); 
	}
}

Number* Grid::getNumberAtPosition(Vector2Int* position)
{
	return numberAtPosition[getMapIndex(position)];
}
