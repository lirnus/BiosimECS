#include <iostream>
#include <chrono>

#include "components.h"
#include "World.h"
#include "simulator_funcs.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";
	auto start = std::chrono::high_resolution_clock::now();
 

	// create new folder directory
	bs::initFolderDir("C:/Users/ochse/Documents/STUDIUM/5. Semester/Biosim v2");

	// initialize function lookup-tables
	initFuncTable();
	initSelectionFuncTable();
	if (deterministic) { randomengine->engine.seed(seeed); }

	// initialize the positions of barriers in the world
	initBarriers();

	// run the simulation
	simulateGenerations();

	auto simulated = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = simulated - start;
	std::cout << "time elapsed after simulation finished: " << elapsed.count() << " seconds\n";

	// render GIFs if told so
	if (createGIF == "every" || createGIF == "selected" || createGIF == "last" || createGIF == "first&last") { renderGIFs(); }

	
	auto end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "time elapsed after rendering finished: " << elapsed.count() << " seconds\n";
	std::cout << "program finished\n";

	return 0;
}