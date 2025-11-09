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
		GOLEFT_GRADIENT,
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
	//enum NeuronTypes : uint8_t {// Has to be sorted: sensors, internals, actions
	//	sX_POS, // 0U
	//	sY_POS,
	//	sPOP_DENSITY_FWD,
	//	sAGE,
	//	iINT_1, // 4U
	//	iINT_2,
	//	iINT_3,
	//	aMOVE_W,	// 7U
	//	aMOVE_E,
	//	aMOVE_N,
	//	aMOVE_S,
	//	NUM_NEURONS // 11U
	//};
	// vector tracking the indexation of the 3 neuron "classes"
	//static constexpr std::array<int, 4> neuronClasses = { 0, 4, 7, 11 };
	extern std::vector<int> neuronClasses;

	// PARAMETERS ////////////////////////////////////////////////////////////////////////////////////

	// world parameters
	static constexpr size_t MAX_ADJ = 25; // maximum number of adjacencies/connections (source/sink) of one neuron to another.
											// controls the size of fwd/bwd_adjacency lists etc. This value is not big enough to cover all possible
											// connection cases for arbitrary genome sizes, but within probable reason, it should never cause problems.
	static constexpr size_t MAX_GENES = 1024; // max number of total genes
	static constexpr size_t MAX_NEURONS = _UI8_MAX; // 0..255
	/*static constexpr int gridsizeY = 100;
	static constexpr int gridsizeX = 100;
	static constexpr int numberOfGenes = 10;
	static constexpr int numberOfPixies = 500;
	static constexpr int numberOfGenerations = 200;
	static constexpr int numberOfSimSteps = 70;
	static constexpr uint8_t selectionCriterium = GOLEFT_GRADIENT; // key for SelCrit-lookup table; see enum SelCrit
	static constexpr uint8_t Barriers_Key = NO_BARRIERS; // see environment.h enum Barriers
	static constexpr uint8_t Interactives_Key = NO_INTERACTIVES; // see environment.h enum Interactives; controls food placement etc.*/

	struct WorldParams {
		int gridSizeY{ 100 };
		int gridSizeX{ 100 };
		int numberOfGenes{ 10 };
		int numberOfPixies{ 500 };
		int numberOfGenerations{ 40 };
		int numberOfSimSteps{ 50 };
		uint8_t selectionCriterium{ 0 };
		uint8_t Barriers_Key{ 0 };
		uint8_t Interactives_Key{ 0 };
	};

	// simulation settings
	//static constexpr bool startingPopulation = false; // starting with a provided list of genomes
	//static constexpr std::string_view startingPop_path = ""; // filepath to extract genomes from

	struct SimulationParams {
		bool startingPopulation{ false };
		std::string_view startingPop_path{ "" };
	};

	// continuous simulation parameters


	// population parameters
	//static constexpr bool blockedByOtherPixies = true;
	//extern bool geneticDrift;
	//static constexpr int pixies_per_genome = 1;
	//extern std::string_view fitnessUpdates;
	//extern bool chooseParentsByFitness; // if True, then fitness plays a role in how likely a pixie will reproduce

	struct PopulationParams {
		bool blockedByOtherPixies{ true };
		int pixies_per_genome{ 1 }; // this value is currently no being used!
	};

	// pixie parameters
	//static constexpr float mutationRate = 0.001; //usual value ~0.001 or 0.0001
	//static constexpr float weight_factor = 2; //range of connection weight encoded by DNA. Range is {-weight_f..+weight_f}
	//static constexpr float defaultSearchRadius = 5.; //default searchradius for pixies when interacting with environment

	struct PixieParams {
		float mutationRate{ 0.001 };
		float weight_factor{ 2 };
		float defaultSearchRadius{ 5 };
	};


	// Neurons

	struct ActiveNeurons {
		//s
		bool xPos;
		bool yPos;
		bool popDensityFwd;
		bool age;
		//i
		int numInternals;
		//a
		bool moveW;
		bool moveE;
		bool moveN;
		bool moveS;
	};

	// analytics
	//static constexpr bool calc_pop_stats = true;
	//static constexpr std::string_view save_metagenome = "every"; //save the metagenome: "none", "first&last", "every" or "selected"
	//static constexpr int saveMetagenomeEvery = 1;
	//static constexpr std::array<int, 11> saveMetagenomeFor = { 2, 3, 5, 10, 20, 50, 100, 200, 300, 400, 500 };
	//extern std::vector<float> diversity_over_time;
	//extern std::vector<float> survival_over_time;

	struct AnalyticsParams {
		bool calc_pop_stats{ true };
		std::string save_metagenome{ "first&last" };
		int saveMetagenomeEvery{ 1 };
		std::vector<int> saveMetagenomeFor{ 2, 3, 5, 10, 20, 50, 100, 200, 300, 400, 500 };
	};

	// render settings
	//static constexpr std::string_view	 createGIF = "first&last"; //"none", "every", "selected" or "first&last"
	//static constexpr int				 GIF_resolution = 10; //number of pixels = width of a cell
	//static constexpr int				 createGIFevery = 1; //generate a GIF every ... generations
	//static constexpr std::array<int, 11> createGIFfor = { 2, 3, 5, 10, 20, 50, 100, 200, 300, 400, 500 }; //generate a GIF for selected generations
	//static constexpr int				 color_variation = 20; //regulates how similar the color of two mutated lineages are

	struct RenderParams {
		std::string createGIF{ "first&last" };
		int GIF_resolution{ 10 };
		int createGIFevery{ 1 };
		std::vector<int> createGIFfor{ 2, 3, 5, 10, 20, 50, 100, 200, 300, 400, 500 };
		int color_variation{ 20 };
	};

	// file system paths etc.
	//static constexpr std::string_view baseDir = "C:/Users/ochse/Documents/STUDIUM/5. Semester/Biosim v2";
	extern std::string folder_dir;
	extern std::string cwd_str;
	extern double simulationTime;
	extern double renderingTime;
	void initFolderDir();
	void initFolderDir(std::string baseDir);
	
	struct BaseDir {
		std::string baseDir{ "C:/Users/ochse/Documents/STUDIUM/5. Semester/Biosim v2" };
	};


	// random engine
	//static constexpr bool deterministic = true; // switch to control if the simulation should run with a predefined random-seed
	//static constexpr int seeed = 1; // seed for deterministic behaviour
	extern Random* randomengine;

	struct RngParams {
		bool deterministic{ true };
		int seeed{ 42 }; 
	};

	// simStep "clock" to track the age of a generation
	extern int generationAge;

	extern WorldParams* worldParams;
	extern SimulationParams* simParams;
	extern PopulationParams* popParams;
	extern PixieParams* pixParams;
	extern ActiveNeurons* activeNeurons;
	extern AnalyticsParams* analParams;
	extern RenderParams* renderParams;
	extern BaseDir* basedir_struct;
	extern RngParams* rngParams;
	void initParameters(const std::string& filename);
}