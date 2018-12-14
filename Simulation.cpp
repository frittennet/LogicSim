#include <Windows.h> 
#include <thread>
#include <mutex>
#include <unordered_map> 
#include <vector> 

#include "Number.h"
#include "Node.h" 
#include "NumberAction.h"
#include "Simulation.h"
#include "LogicSim.h" 

Simulation::Simulation(int numGrids, int numNodes) {
	tickIndex = 0;
	tickInterval = 1000;
	isPaused = true;
	stopThread = false; 
	currentInputNumberIndex = 0;
	numInputNumbers = 0;
	this->numGrids = numGrids; 
	this->numNodes = numNodes; 

	grids = new Grid*[numGrids]; 
	nodes = new Node*[numNodes]; 
	
} 

Simulation::~Simulation() {
	mutex.lock(); 
	if (simulationThread.joinable()) {
		simulationThread.join();
	}
	mutex.unlock(); 
	for (int i = 0; i < numNodes; i++) {
		delete nodes[i]; 
	}
	
	for (int i = 0; i < numGrids; i++) {
		delete grids[i];
	}

	delete[] nodes; 
	delete[] grids; 

	

}

void Simulation::addGrid(Grid * grid)
{
	grids[grid->id] = grid; 
}

void Simulation::addNumber(Number * number)
{
	numberList.insert(std::pair<int, Number*>(number->id, number)); 
}

void Simulation::removeNumber(Number * number)
{
	numberList.erase(number->id); 
	delete number; 
}

void Simulation::addNode(Node* node) {
	nodes[node->id] = node; 
	if (node->type == NodeType::GAMEINPUT) {
		rootInputNode = (NodeGameInput*)node; 
	}
	if (node->type == NodeType::GAMEOUTPUT) {
		rootOutputNode = (NodeGameOutput*)node; 
	}
}

void Simulation::start()
{
	printf("Starting Simulation \n"); 
	mutex.lock(); 
	isPaused = false; 
	simulationThread = std::thread(&Simulation::threadEntryPoint, this);
	// simulationThread.detach(); 
	mutex.unlock(); 
}

void Simulation::pause()
{
	mutex.lock();
	isPaused = true; 
	mutex.unlock();
}

void Simulation::reset() {
	if (simulationThread.joinable()) {
		printf("Joining Threads \n"); 
		stopThread = true; 
		simulationThread.join();
		stopThread = false; 
	}
	tickIndex = 0;
	isPaused = true;
	currentInputNumberIndex = 0;
	outputs.clear();
	int iteration = 0; 
	std::unordered_map<int, Number*>::iterator it2 = numberList.begin();
	while (it2 != numberList.end()) {
		Number* number = it2->second; 
		number->grid->removeNumber(number);
		delete number; 
		it2++; 
	}
	numberList.clear(); 
}

void Simulation::stop()
{
	reset(); 
}

void Simulation::setInputNumbers(int * numbers, int numNumbers)
{
	reset(); 
	inputNumbers = numbers; 
	numInputNumbers = numNumbers; 
	currentInputNumberIndex = 0; 
}

int* Simulation::getOutputNumbers()
{
	if (outputs.size() > 0) {
		return outputs.data(); 
	}
	return nullptr; 
}

void Simulation::setTickInterval(int tickInterval)
{
	mutex.lock(); 
	this->tickInterval = tickInterval; 
	mutex.unlock(); 
}

void Simulation::threadEntryPoint()
{
	while (!stopThread) {
		mutex.lock(); 
		if (!isPaused) {
			printf("Tick \n"); 
			tick(); 
		}
		mutex.unlock(); 
		Sleep(tickInterval); 
	}
}

void Simulation::tick()
{
	Vector2Int forward = rootInputNode->position + Vector2Int::fromDirection(rootInputNode->direction); 
	if (rootGrid->getNumberAtPosition(&rootInputNode->position) == nullptr && rootGrid->getNumberAtPosition(&forward) == nullptr) {
		if (currentInputNumberIndex < numInputNumbers) {
			Number* newInputNumber = new Number(rootGrid);
			newInputNumber->value = inputNumbers[currentInputNumberIndex];
			newInputNumber->position = rootInputNode->position;
			newInputNumber->direction = rootInputNode->direction;
			Vector2Int forward = rootInputNode->position + Vector2Int::fromDirection(rootInputNode->direction);
			numberList.insert(std::pair<int, Number*>(newInputNumber->id, newInputNumber));
			rootGrid->setNumber(newInputNumber);
			newInputNumber->currentAction = new NumberActionSpawnMove(newInputNumber, rootInputNode->position, forward); 

			currentInputNumberIndex++;; 
		}
	}

	std::unordered_map<int, Number*>::iterator it = numberList.begin();
	while (it != numberList.end()) {
		Number* number = it->second;
		number->tick();
		it++; 
	}

	std::vector<APINumber> apiNumbers; 
	std::unordered_map<int, Number*>::iterator it2 = numberList.begin();
	while (it2 != numberList.end()) {
		Number* number = it2->second;
		if (number->currentAction != nullptr) {
			bool executed = number->currentAction->execute(number);
			if (executed) {
				printf("Number %i -> %i \n", number->id, number->currentAction->type);
				APINumber apiNumber = APINumber(number->id, number->grid->id, number->position, number->direction, number->value, number->currentAction->type);
				apiNumbers.push_back(apiNumber);
			}
		}
		it2++; 
	}

	APISimulationState* currentState; 
	if (apiNumbers.size() > 0) { 
		APINumber* numbers = new APINumber[apiNumbers.size()]; 
		std::copy(apiNumbers.begin(), apiNumbers.end(), numbers); 
		currentState = new APISimulationState(numbers, currentInputNumberIndex);
	}
	else {
		currentState = new APISimulationState(nullptr, currentInputNumberIndex);
	}

	
	apiSimulation->addState(currentState); 
}
