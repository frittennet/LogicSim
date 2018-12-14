#pragma once

#include <vector> 

#include "NodeType.h" 
#include "Vector2Int.h" 
#include "Direction.h"

#define DllExport   __declspec( dllexport )

class Simulation;

class DllExport APINode
{
public:
	int id;
	Vector2Int position;
	Direction direction;
	NodeType type;
	int gridId;
	int referenceNodeId; //  NodeCustom, NodeOutput 

	APINode(int id, Vector2Int position, Direction direction, NodeType type, int gridId, int referenceNodeId) {
		this->id = id; 
		this->position = position; 
		this->direction = direction; 
		this->type = type; 
		this->gridId = gridId; 
		this->referenceNodeId = referenceNodeId; 
	}

};

class DllExport APIGrid
{
public:
	int id;

	APIGrid(int id) {
		this->id = id; 
	}

};

class DllExport APISimulationDefinition
{
public:
	APIGrid* grids;
	APINode* nodes;
	int numGrids;
	int numNodes;

	APISimulationDefinition(APIGrid grids[], APINode nodes[], int numGrids, int numNodes) {
		this->grids = grids; 
		this->nodes = nodes; 
		this->numGrids = numGrids; 
		this->numNodes = numNodes; 
	}

	~APISimulationDefinition(); 

};

enum DllExport APINumberAction {
	ACTION_NONE, ACTION_MOVE, ACTION_DELETE, ACTION_SPAWN, ACTION_MERGE
};

class DllExport APINumber
{
public:
	int id;
	int gridId;
	Vector2Int position;
	Direction direction;
	int value;
	APINumberAction lastAction; 

	APINumber() { }; 
	APINumber(int id, int gridId, Vector2Int position, Direction direction, int value, APINumberAction lastAction) {
		this->id = id; 
		this->gridId = gridId; 
		this->position = position; 
		this->direction = direction; 
		this->value = value; 
		this->lastAction = lastAction; 
	}

};

class DllExport APISimulationState
{
public:
	~APISimulationState();
	APINumber* numbers;
	int currentInputNumberIndex;

	APISimulationState(APINumber numbers[], int currentInputNumberIndex) {
		this->numbers = numbers; 
		this->currentInputNumberIndex = currentInputNumberIndex; 
	}

};

class DllExport APISimulation
{
private:
	Simulation* simulation; 
public:
	APISimulation(Simulation* simulation);
	~APISimulation();

	int getTickIndex();
	APISimulationState* getState(int tick);
	void start();
	void pause();
	void stop();
	void setInputNumbers(int numbers[], int numNumbers); 
	void setTickInterval(int tickInterval); 
	int getTickInterval(int tickInterval); 
};

class __declspec(dllexport) API { 
public: 
	static APISimulation* createSimulation(APISimulationDefinition* apiSimulationDefinition);
	static void destroySimulation(APISimulation* simulation);
};