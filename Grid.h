#pragma once

#include "Vector2Int.h"
#include "Node.h" 
#include "Number.h"

#include <unordered_map> 

class Simulation; 

class Grid
{
private:
	
public:
	std::unordered_map<int64_t, Node*> nodeAtPosition;
	std::unordered_map<int64_t, Number*> numberAtPosition;
	std::vector<Node*> inputNodes[4]; 

	Simulation* sim; 

	int id; 

	Grid(Simulation* sim);
	~Grid();

	static int64_t getMapIndex(Vector2Int* position); 
	void setNode(Node* node); 
	void removeNode(Node* node); 
	Node* getNodeAtPosition(Vector2Int* position); 
	void setNumber(Number* number);
	void removeNumber(Number* number);
	Number* getNumberAtPosition(Vector2Int* position);

};

