#include <thread> 

#include "LogicSim.h"
#include "Simulation.h"

APISimulation* API::createSimulation(APISimulationDefinition* apiSimulationDefinition)
{
	printf("Initializing \n"); 
	Simulation* simulation = new Simulation(apiSimulationDefinition->numGrids, apiSimulationDefinition->numNodes);
	printf("Grids : %i , Nodes : %i \n", apiSimulationDefinition->numGrids, apiSimulationDefinition->numNodes);

	printf("Initializing Grid Objects \n"); 
	for(int i=0;i<apiSimulationDefinition->numGrids;i++) { 
		APIGrid apiGrid = apiSimulationDefinition->grids[i];
		printf("Grid %i \n", apiGrid.id);
		Grid* grid = new Grid(simulation);
		grid->id = apiGrid.id;
		if (apiGrid.id == 0) {
			simulation->rootGrid = grid;
		}
		simulation->addGrid(grid); 
	}
	
	printf("Initializing Nodes \n"); 
	for (int i = 0; i < apiSimulationDefinition->numNodes; i++) {
		APINode apiNode = apiSimulationDefinition->nodes[i];
		Grid* grid = simulation->grids[apiNode.gridId];
		printf("Node %i Node->grid->id %i \n", apiNode.id, grid->id);
		Grid* subGrid;
		Node* ref;
		Node* node = nullptr;
		switch (apiNode.type) {
		case NodeType::CUSTOM:
			subGrid = simulation->grids[apiNode.referenceNodeId];
			node = new NodeCustom(grid, subGrid);
			break;
		case NodeType::GAMEINPUT:
			node = new NodeGameInput(grid);
			break;
		case NodeType::GAMEOUTPUT:
			node = new NodeGameOutput(grid);
			break;
		case NodeType::MINUS:
			node = new NodeMinus(grid);
			break;
		case NodeType::NODE_INPUT:
			node = new NodeInput(grid);
			break;
		case NodeType::OUTPUT:
			ref = simulation->nodes[apiNode.referenceNodeId];
			node = new NodeOutput(grid, (NodeCustom*)ref);
			break;
		case NodeType::SMALLERTHAN:
			node = new NodeSmallerThan(grid);
			break;
		}

		node->id = apiNode.id;
		node->position = apiNode.position; 
		node->direction = apiNode.direction; 
		node->grid = grid; 
		node->type = apiNode.type; 
		grid->setNode(node);
		simulation->addNode(node);
	}

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
	std::vector<APISimulationState*>::iterator it2 = states.begin();
	while (it2 != states.end()) {
		delete *it2; 
		it2++; 
	}
	delete simulation;
}

int APISimulation::getTickIndex()
{
	return simulation->tickIndex;
}

void APISimulation::addState(APISimulationState * state)
{
	this->states.push_back(state); 
}

APISimulationState* APISimulation::getState(int tick)
{
	return states[tick];
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

void APISimulation::setInputNumbers(int numbers[], int numNumbers)
{
	simulation->setInputNumbers(numbers, numNumbers);
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
