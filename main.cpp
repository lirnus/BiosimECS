#include <iostream>
#include <chrono>

#include "simulator_funcs.h"


int main() {
	using namespace bs;

	// BEFORE THE SIMULATION ////////////////////////////////////////////////////////////

	// start the timer
	std::cout << "program is running...\n\n";
	auto start = std::chrono::high_resolution_clock::now();
	
	// initialize simulation parameters
	std::string configFile = "simconfig.ini";
	initParameters(configFile);

	// create new folder directory
	initFolderDir(static_cast<std::string>(basedir_struct->baseDir));

	// initialize function lookup-tables
	initNeuronFuncTable();
	initSelectionFuncTable();

	// set a seed for RNGs
	if (rngParams->deterministic) { randomengine->engine.seed(rngParams->seeed); }

	// initialize the future positions of barriers in the world
	initBarriers();


	// RUNNING THE SIMULATION ///////////////////////////////////////////////////////////

	// different simulation modes come into question in the future
	simulateGenerations();


	// AFTER THE SIMULATION /////////////////////////////////////////////////////////////

	// take the time that simulating took
	auto simulated = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = simulated - start;
	simulationTime = elapsed.count();
	std::cout << "time elapsed for simulation: " << simulationTime << " seconds\n";

	// plot some stuff for popGen analysis (mullerplot, survival rates, etc.)
	std::string_view sm = analParams->save_metagenome;
	if (sm == "every" || sm == "selected" || sm == "last" || sm == "first&last") {
		popGenAnalysis();
	}

	// render GIFs if told so
	std::string_view cg = renderParams->createGIF;
	if (cg == "every" || cg == "selected" || cg == "last" || cg == "first&last") { 
		renderGIFs(); 
	}

	// take the time that rendering took
	auto end = std::chrono::high_resolution_clock::now();
	elapsed = end - simulated;
	renderingTime = elapsed.count();
	std::cout << "time elapsed for rendering: " << renderingTime << " seconds\n";

	// save the Metadata / Log for this simulation
	saveMetaData(configFile);

	// call it a day!
	std::cout << "program finished\n";

	/////////////////////////////////////////////////////////////////////////////////////

	return 0;
}