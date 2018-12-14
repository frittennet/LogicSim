#include <Windows.h> 

#include <stdio.h> 

#include "common.h" 
#include "Grid.h" 
#include "LogicSim.h" 
#include "Simulation.h"
#include "NumberAction.h" 
#include "Direction.h" 
#include "Vector2Int.h" 


#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	return TRUE;
}

void test() {
	APIGrid grids[] = { APIGrid(0), APIGrid(1) };
	APINode nodes[] = { 
		APINode(0, Vector2Int(10, 10), Direction::RIGHT, NodeType::GAMEINPUT, 0, 0), 
		APINode(1, Vector2Int(90, 10), Direction::RIGHT, NodeType::GAMEOUTPUT, 0, 0), 
		APINode(2, Vector2Int(50, 10), Direction::RIGHT, NodeType::SMALLERTHAN, 0, 0), 
		APINode(3, Vector2Int(60, 10), Direction::RIGHT, NodeType::CUSTOM, 0, 1), 
		APINode(4, Vector2Int(90, 10), Direction::RIGHT, NodeType::OUTPUT, 1, 3), 
		APINode(5, Vector2Int(10, 10), Direction::RIGHT, NodeType::NODE_INPUT, 1, 0),
		APINode(6, Vector2Int(10, 16), Direction::RIGHT, NodeType::NODE_INPUT, 1, 0) 
	};
	
	APINumber gridNumbers[] = { 
		APINumber(0, 0, Vector2Int(0, 0), Direction::RIGHT, 0, APINumberAction::ACTION_SPAWN), 
		APINumber(1, 0, Vector2Int(1, 0), Direction::DOWN, 0, APINumberAction::ACTION_SPAWN), 
		APINumber(2, 0, Vector2Int(1, 1), Direction::LEFT, 0, APINumberAction::ACTION_SPAWN),
		APINumber(3, 0, Vector2Int(0, 1), Direction::UP, 0, APINumberAction::ACTION_SPAWN)
	}; 
	
	int inputNumbers[] = { -1, 0, 1 }; 

	APISimulationDefinition d = APISimulationDefinition(grids, 2, nodes, 7, gridNumbers, 4, inputNumbers, 3);

	APISimulation* s = API::createSimulation(&d);

	s->start();
	s->setTickInterval(0);

	int n = 0;
	int lastTickIndex = 0; 
	while (true) {
		debugPrint("Main Thread Test \n");
		Sleep(1000);
		int index = s->getTickIndex(); 
		printf("Ticks Per Second : %i \n", (index - lastTickIndex));;
		lastTickIndex = index;
		APISimulationState* state = s->getState(index); 
		debugPrint("Getting State for tick : %i \n", index); 
		if (state != nullptr && state->numbers != nullptr) {
			printf("Number [%i] position => [%i, %i] \n", state->numbers[0].id, state->numbers[0].position.x, state->numbers[0].position.y); 
		}
		if (n == 60) {
			API::destroySimulation(s);
			break;
		}
		n++;
	}
}

int main(void) {
	
	
	test(); 

	while (true) {
		Sleep(1000); 
	}

}
