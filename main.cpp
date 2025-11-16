#include <iostream>
#include <chrono>

#include "components.h"
#include "World.h"
#include "simulator_funcs.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";
	auto start = std::chrono::high_resolution_clock::now();
	
	std::string configFile = "simconfig.ini";
	bs::initParameters(configFile);

	// create new folder directory
	bs::initFolderDir(static_cast<std::string>(basedir_struct->baseDir));

	// initialize function lookup-tables
	initFuncTable();
	initSelectionFuncTable();
	if (rngParams->deterministic) { randomengine->engine.seed(rngParams->seeed); }

	// initialize the positions of barriers in the world
	initBarriers();

	// run the simulation
	simulateGenerations();

	auto simulated = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = simulated - start;
	simulationTime = elapsed.count();
	std::cout << "time elapsed for simulation: " << simulationTime << " seconds\n";

	// render GIFs if told so
	std::string_view cg = renderParams->createGIF;
	if (cg == "every" || cg == "selected" || cg == "last" || cg == "first&last") { 
		renderGIFs(); 
	}
	// plot some stuff for popGen analysis (mullerplot, survival rates, etc.)
	std::string_view sm = analParams->save_metagenome;
	if (sm == "every" || sm == "selected" || sm == "last" || sm == "first&last") {
		popGenAnalysis();
	}

	
	auto end = std::chrono::high_resolution_clock::now();
	elapsed = end - simulated;
	renderingTime = elapsed.count();
	std::cout << "time elapsed for rendering: " << renderingTime << " seconds\n";

	saveMetaData(configFile);
	std::cout << "program finished\n";

	return 0;
}