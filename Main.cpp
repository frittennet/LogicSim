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
	APINode nodes[] = { APINode(0, Vector2Int(0, 0), Direction::RIGHT, NodeType::GAMEINPUT, 0, 0) , APINode(1, Vector2Int(90, 10), Direction::RIGHT, NodeType::GAMEOUTPUT, 0, 0) };

	APISimulationDefinition d = APISimulationDefinition(grids, nodes, 2, 2);

	APISimulation* s = API::createSimulation(&d);

	int numbers[10];
	for (int i = 0; i < 10; i++) {
		numbers[i] = i;
	}

	s->setInputNumbers(numbers, 100);
	s->start();
	s->setTickInterval(0);

	int n = 0;
	int lastTickIndex = 0; 
	while (true) {
		debugPrint("Main Thread Test \n");
		Sleep(1000);
		printf("Ticks Per Second : %i \n", (s->getTickIndex() - lastTickIndex));; 
		lastTickIndex = s->getTickIndex(); 
		APISimulationState* state = s->getState(s->getTickIndex()); 
		if (state != nullptr) {
			printf("Number [%i] positoin => [%i, %i] \n", state->numbers[0].id, state->numbers[0].position.x, state->numbers[0].position.y); 
		}
		if (n == 60000) {
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
