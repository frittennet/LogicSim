#pragma once

#include <thread> 
#include <mutex> 

#include "Vector2Int.h"
#include "Node.h"
#include "Grid.h"

#include <unordered_map> 

class APISimulation; 
class APISimulationState;

using namespace std;

class Simulation {
private:
	int* inputNumbers;
	int numInputNumbers;
	std::mutex mutex; 
	std::thread simulationThread; 
	bool stopThread; 
	std::vector<APISimulationState*> states; 

	void tick(); 
	void threadEntryPoint();

public: 
	bool isPaused; 
	int tickIndex; 
	int tickInterval; 
	int numGrids; 
	int numNodes; 
	
	Simulation(int numGrids, int numNodes);
	~Simulation();

	Grid* rootGrid; 
	Grid** grids; 
	Node** nodes; 
	std::unordered_map<int, Number*> numberList; 
	NodeGameInput* rootInputNode; 
	NodeGameOutput* rootOutputNode; 
	APISimulation* apiSimulation; 

	int currentInputNumberIndex; 
	std::vector<int> outputs; 
	
	APISimulationState* getState(int tick); 
	void addGrid(Grid* grid);
	void addNode(Node* node); 
	void addNumber(Number* number); 
	void spawnNumber(APINumber number); 
	void removeNumber(Number* number); 
	void start(); 
	void pause(); 
	void stop(); 
	void setInputNumbers(int* numbers, int numNumbers); 
	int* getOutputNumbers(); 
	void setTickInterval(int tickInterval); 

};
