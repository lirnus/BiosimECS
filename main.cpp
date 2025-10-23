#include <iostream>
#include <chrono>

#include "components.h"
#include "World.h"
#include "simulator_funcs.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";
	auto start = std::chrono::high_resolution_clock::now();
	
	bs::initParameters("simconfig.ini");

	// create new folder directory
	bs::initFolderDir(static_cast<std::string>(baseDir));

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
	simulationTime = elapsed.count();
	std::cout << "time elapsed for simulation: " << simulationTime << " seconds\n";

	// render GIFs if told so
	if (createGIF == "every" || createGIF == "selected" || createGIF == "last" || createGIF == "first&last") { 
		renderGIFs(); 
	}
	// plot some stuff for popGen analysis (mullerplot, survival rates, etc.)
	if (save_metagenome == "every" || save_metagenome == "selected" || save_metagenome == "last" || save_metagenome == "first&last") {
		popGenAnalysis();
	}

	
	auto end = std::chrono::high_resolution_clock::now();
	elapsed = end - simulated;
	renderingTime = elapsed.count();
	std::cout << "time elapsed for rendering: " << renderingTime << " seconds\n";

	saveMetaData();
	std::cout << "program finished\n";

	return 0;
}