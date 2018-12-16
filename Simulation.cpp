#include <Windows.h> 
#include <thread>
#include <mutex>
#include <unordered_map> 
#include <vector> 

#include "common.h" 

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

	for (auto&& i : numberList)
	{
		numberPool.deleteElement(i.second); 
	}
	
	std::vector<APISimulationState*>::iterator it2 = states.begin();
	while (it2 != states.end()) {
		delete *it2;
		it2++;
	}
	
	for (int i =0 ; i < numGrids; i++) {
		gridPool.deleteElement(grids[i]); 
	}

	for (int i = 0; i < numNodes; i++) {
		nodePool.deallocate(nodes[i]); 
	}

	delete[] grids; 
	delete[] nodes; 

}

APISimulationState * Simulation::getState(int tick)
{
	while (states.size() <= tick) {
		Sleep(1);
	}
	APISimulationState* state = states.size() > tick ? states[tick] : nullptr;
	return state;
}

void Simulation::addGrid(Grid* grid)
{
	grids[grid->id] = grid;
	if (grid->id == 0) {
		rootGrid = grid; 
	}
}

void Simulation::addNumber(Number* number)
{
	numberList.insert(std::pair<int, Number*>(number->id, number)); 
	this->grids[number->grid->id]->setNumber(number); 
}

void Simulation::removeNumber(Number* number)
{
	numberList.erase(number->id); 
	numberPool.deleteElement(number); 
}

void Simulation::addNode(Node* node) {
	nodes[node->id] = node;
	if (node->type == NodeType::GAMEINPUT) {
		rootInputNode = (NodeGameInput*)node; 
	}
	if (node->type == NodeType::GAMEOUTPUT) {
		rootOutputNode = (NodeGameOutput*)node; 
	}
	node->grid->setNode(node); 
}

void Simulation::start()
{
	debugPrint("Starting Simulation \n");
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

void Simulation::stop()
{
	if (simulationThread.joinable()) {
		debugPrint("Joining Threads \n");
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
		numberPool.deleteElement(number); 
		it2++;
	}
	numberList.clear();
}

void Simulation::setInputNumbers(int * numbers, int numNumbers)
{
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
	this->tickInterval = tickInterval;
}

void Simulation::threadEntryPoint()
{
	while (!stopThread) {
		if (!isPaused) {
			mutex.lock();
			debugPrint("Tick \n");
			tick();
			mutex.unlock();
		}
		Sleep(tickInterval);
	}
}

void Simulation::spawnNumber(APINumber apiNumber) {
	Grid* grid = grids[apiNumber.gridId];
	Number* newInputNumber = numberPool.newElement(grid); 
	newInputNumber->value = apiNumber.value;
	newInputNumber->position = apiNumber.position;
	newInputNumber->direction = apiNumber.direction;
	Vector2Int forward = apiNumber.position + Vector2Int::fromDirection(apiNumber.direction);
	this->addNumber(newInputNumber);
	newInputNumber->currentAction = new NumberActionSpawnMove(newInputNumber, apiNumber.position, forward);
}

void Simulation::tick()
{
	Vector2Int forward = rootInputNode->position + Vector2Int::fromDirection(rootInputNode->direction); 
	Number* numberAtInputNode = rootGrid->getNumberAtPosition(&rootInputNode->position); 
	debugPrint("NumberAtInputNode 0x%08x : 0x%08x \n", rootInputNode, numberAtInputNode);
	if (numberAtInputNode == nullptr) {
		if (currentInputNumberIndex < numInputNumbers) {
			spawnNumber(APINumber(0, rootGrid->id, rootInputNode->position, rootInputNode->direction, inputNumbers[currentInputNumberIndex], APINumberAction::ACTION_SPAWN)); 
			currentInputNumberIndex++;;
		}
	}

	debugPrint("NumberList Count = %i \n", numberList.size()); 
	debugPrint("Grid[0] Number Count = %i \n", grids[0]->numberAtPosition.size());
	std::unordered_map<int, Number*>::iterator it = numberList.begin();
	while (it != numberList.end()) {
		Number* number = it->second;
		if (number->deleteNextTick) {
			it = this->numberList.erase(it); 
			number->grid->removeNumber(number); 
			numberPool.deleteElement(number); 
		}
		else {
			number->tick();
			it++;
		}
	}

	std::vector<APINumber> apiNumbers;
	std::unordered_map<int, Number*>::iterator it2 = numberList.begin();
	while (it2 != numberList.end()) {
		Number* number = it2->second;
		debugPrint("Number[%i].currentAction = 0x%08x \n", number->id, number->currentAction); 
		if (number->currentAction != nullptr) {
			bool executed = number->currentAction->execute(number);
			if (executed) {
				debugPrint("Number %i -> %i \n", number->id, number->currentAction->type);
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

	this->states.push_back(currentState);

	debugPrint("Added State %i \n", this->states.size());
	tickIndex++;
}
