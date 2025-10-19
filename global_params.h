#pragma once
#include <string_view>
#include <vector>
#include <set>
#include <iostream>
#include <array>

#include "Random.h"

namespace bs {
	// This file contains global variables that are intended to be accessible from everywhere in the code

	static constexpr double PI = 3.141592653589;

	// enums for selection criteria, environment options and neurons
	enum SelCrit : uint8_t {
		NO_SELECTION,
		KILLRIGHTHALF,
		NUM_SELCRIT
	};
	enum Barriers : uint8_t {
		NO_BARRIERS,
		VERTICAL_BARRIER,
		HORIZONTAL_BARRIER
	};
	enum Interactives : uint8_t {
		NO_INTERACTIVES,
		SPARSE_FOOD,
		DENSE_FOOD,
		FOUR_FLAGS
	};
	enum NeuronTypes : uint8_t {// Has to be sorted: sensors, internals, actions
		sX_POS, // 0U
		sY_POS,
		sPOP_DENSITY_FWD,
		sAGE,
		iINT_1, // 4U
		iINT_2,
		iINT_3,
		aMOVE_W,	// 7U
		aMOVE_E,
		aMOVE_N,
		aMOVE_S,
		NUM_NEURONS // 11U
	};
	// vector tracking the indexation of the 3 neuron "classes"
	static constexpr std::array<int, 4> neuronClasses = { 0, 4, 7, 11 };

	// PARAMETERS ////////////////////////////////////////////////////////////////////////////////////

	// world parameters
	static constexpr int gridsizeY = 30;
	static constexpr int gridsizeX = 30;
	static constexpr int numberOfGenes = 10;
	static constexpr int numberOfPixies = 300;
	static constexpr int numberOfGenerations = 40;
	static constexpr int numberOfSimSteps = 50;
	static constexpr uint8_t selectionCriterium = NO_SELECTION; // key for SelCrit-lookup table; see enum SelCrit
	static constexpr uint8_t Barriers_Key = NO_BARRIERS; // see environment.h enum Barriers
	static constexpr uint8_t Interactives_Key = NO_INTERACTIVES; // see environment.h enum Interactives; controls food placement etc.

	// simulation settings
	static constexpr bool startingPopulation = false; // starting with a provided list of genomes
	static constexpr std::string_view startingPop_path = ""; // filepath to extract genomes from

	// continuous simulation parameters

	// population parameters
	static constexpr bool blockedByOtherPixies = true;
	//extern bool geneticDrift;
	static constexpr int pixies_per_genome = 1;
	//extern std::string_view fitnessUpdates;
	//extern bool chooseParentsByFitness; // if True, then fitness plays a role in how likely a pixie will reproduce

	// pixie parameters
	static constexpr float mutationRate = 0.0; //usual value ~0.001 or 0.0001
	static constexpr float weight_factor = 2; //range of connection weight encoded by DNA. Range is {-weight_f..+weight_f}
	static constexpr float defaultSearchRadius = 5.; //default searchradius for pixies when interacting with environment

	// analytics
	//extern bool calc_diversity;
	//extern bool calc_survivalrate;
	//extern bool create_mullerplot; //generate mullerplot
	//extern std::string save_metagenome; //save the metagenome: "none", "last", "every" or "selected"
	//extern int saveMetagenomeEvery;
	//extern std::set<int> saveMetagenomeFor;
	//extern std::vector<float> diversity_over_time;
	//extern std::vector<float> survival_over_time;

	// render settings
	static constexpr std::string_view	 createGIF = "none"; //"none", "every", "selected", "last" or "first&last"
	static constexpr int				 GIF_resolution = 10; //number of pixels = width of a cell
	static constexpr int				 createGIFevery = 1; //generate a GIF every ... generations
	static constexpr std::array<int, 13> createGIFfor = { numberOfGenerations, 1, 2, 3, 5, 10, 20, 50, 100, 200, 300, 400, 500 }; //generate a GIF for selected generations
	static constexpr int				 color_variation = 20; //regulates how similar the color of two mutated lineages are

	// file system paths etc.
	static constexpr std::string_view baseDir = "C:/Users/ochse/Documents/STUDIUM/5. Semester/Biosim v2";
	extern std::string folder_dir;
	extern std::string cwd_str;
	extern double simulationTime;
	extern double renderingTime;
	void initFolderDir();
	void initFolderDir(std::string baseDir);


	// random engine
	static constexpr bool deterministic = true; // switch to control if the simulation should run with a predefined random-seed
	static constexpr int seeed = 1; // seed for deterministic behaviour
	extern Random* randomengine;

	// simStep "clock" to track the age of a generation
	extern int generationAge;

}