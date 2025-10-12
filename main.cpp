#include <iostream>
#include <chrono>

#include "components.h"
#include "World.h"
#include "simulator_funcs.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";
	auto start = std::chrono::high_resolution_clock::now();
	
	initFuncTable();
	initSelectionFuncTable();
	if (deterministic) { randomengine->engine.seed(seeed); }

	/*World myWorld;
	World* myWorld_ptr = &myWorld;

	for (int i = 0; i < numberOfPixies; i++) {
		spawnPixie(myWorld_ptr);
	}*/

	//myWorld_ptr->printGrid();

	simulateGenerations();
	
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "time elapsed: " << elapsed.count() << " seconds\n";
	std::cout << "program finished\n";

	return 0;
}