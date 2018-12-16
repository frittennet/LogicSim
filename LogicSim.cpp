#include <thread> 

#include "common.h" 

#include "LogicSim.h"
#include "Simulation.h"

APISimulation* API::createSimulation(APISimulationDefinition* apiSimulationDefinition)
{
	debugPrint("Initializing \n"); 
	Simulation* simulation = new Simulation(apiSimulationDefinition->numGrids, apiSimulationDefinition->numNodes);
	debugPrint("Grids : %i , Nodes : %i \n", apiSimulationDefinition->numGrids, apiSimulationDefinition->numNodes);

	debugPrint("Initializing Grid Objects \n"); 
	for(int i=0;i<apiSimulationDefinition->numGrids;i++) { 
		APIGrid apiGrid = apiSimulationDefinition->grids[i];
		debugPrint("Grid %i \n", apiGrid.id);
		Grid* grid = simulation->gridPool.newElement(simulation); 
		grid->id = apiGrid.id;
		simulation->addGrid(grid); 
	}
	
	debugPrint("Initializing Nodes \n"); 
	for (int i = 0; i < apiSimulationDefinition->numNodes; i++) {
		APINode apiNode = apiSimulationDefinition->nodes[i];
		Grid* grid = simulation->grids[apiNode.gridId];
		debugPrint("Node %i Node->grid->id %i \n", apiNode.id, grid->id);
		Grid* subGrid;
		Node* ref;
		Node* nodePtr = simulation->nodePool.allocate(); 
		Node* node = nullptr; 
		switch (apiNode.type) {
		case NodeType::CUSTOM:
			subGrid = simulation->grids[apiNode.referenceNodeId];
			node = new (nodePtr) NodeCustom(grid, subGrid);
			break;
		case NodeType::GAMEINPUT:
			node = new (nodePtr) NodeGameInput(grid);
			break;
		case NodeType::GAMEOUTPUT:
			node = new (nodePtr) NodeGameOutput(grid);
			break;
		case NodeType::MINUS:
			node = new (nodePtr) NodeMinus(grid);
			break;
		case NodeType::NODE_INPUT:
			node = new (nodePtr) NodeInput(grid);
			break;
		case NodeType::OUTPUT:
			ref = simulation->nodes[apiNode.referenceNodeId];
			node = new (nodePtr) NodeOutput(grid, (NodeCustom*)ref);
			break;
		case NodeType::SMALLERTHAN:
			node = new (nodePtr) NodeSmallerThan(grid);
			break;
		}

		node->id = apiNode.id;
		node->position = apiNode.position; 
		node->direction = apiNode.direction; 
		node->grid = grid; 
		node->type = apiNode.type; 
		simulation->addNode(node);
	}

	debugPrint("Initializing Pre-Defined Grid Numbers ( placed before simulation starts ) \n");
	for (int i = 0; i < apiSimulationDefinition->numGridNumbers; i++) {
		APINumber apiNumber = apiSimulationDefinition->gridNumbers[i];
		debugPrint("Setting up Grid Number %i on %i \n", apiNumber.id, apiNumber.gridId);
		simulation->spawnNumber(apiNumber);
	}

	simulation->setInputNumbers(apiSimulationDefinition->inputNumbers, apiSimulationDefinition->numInputNumbers); 

	APISimulation* apiSimulation = new APISimulation(simulation);
	simulation->apiSimulation = apiSimulation;

	return apiSimulation;
}

void API::destroySimulation(APISimulation* simulation)
{
	delete simulation;
}

APISimulation::APISimulation(Simulation* simulation)
{
	this->simulation = simulation;
}

APISimulation::~APISimulation()
{
	simulation->stop();
	delete simulation;
}

int APISimulation::getTickIndex()
{
	return simulation->tickIndex;
}

APISimulationState* APISimulation::getState(int tick)
{
	return simulation->getState(tick); 
}

void APISimulation::start()
{
	simulation->start();
}

void APISimulation::pause()
{
	simulation->pause();
}

void APISimulation::stop()
{
	simulation->stop();
}

void APISimulation::setTickInterval(int tickInterval)
{
	simulation->setTickInterval(tickInterval); 
}

int APISimulation::getTickInterval(int tickInterval)
{
	return simulation->tickInterval; 
}

APISimulationState::~APISimulationState()
{
	if (numbers != nullptr) {
		delete[] numbers; 
	}
}

APISimulationDefinition::~APISimulationDefinition()
{
	
}
