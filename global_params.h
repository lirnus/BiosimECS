#pragma once
#include <string>
#include <vector>
#include <set>
#include <iostream>

#include "Random.h"

namespace bs {
	// This file contains global variables that are intended to be accessible from everywhere in the code

	static constexpr double PI = 3.141592653589;

	// world parameters
	/*extern int gridsizeY;
	extern int gridsizeX;
	extern int numberOfGenes;
	extern int numberOfPixies;
	extern int numberOfGenerations;
	extern int numberOfSimSteps;*/

	static constexpr int gridsizeY = 30;
	static constexpr int gridsizeX = 30;
	static constexpr int numberOfGenes = 4;
	static constexpr int numberOfPixies = 200;
	static constexpr int numberOfGenerations = 40;
	static constexpr int numberOfSimSteps = 30;
	static constexpr uint8_t selectionCriterium = 0; // key for SelCrit-lookup table; see enum SelCrit
	//extern std::string EnvironmentKey;

	// continuous simulation parameters

	// population parameters
	//extern bool blockedByOtherPixies;
	//extern bool geneticDrift;
	//extern float mortalityRate; // obsolete/usless/uniteresting with new fitness system?
	//extern std::string fitnessUpdates;
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
	//extern std::string createGIF; //"none", "every" or "selected"
	//extern std::string renderMode; //"pretty" or "fast"
	//extern int GIF_resolution; //number of pixels = width of a cell
	//extern int createGIFevery; //generate a GIF every ... generations
	//extern std::set<int> createGIFfor; //generate a GIF for selected generations
	static constexpr int color_variation = 20; //regulates how similar the color of two mutated lineages are

	// file system paths etc.
	/*extern std::string folder_dir;
	extern std::string cwd_str;*/

	// random engine
	static constexpr bool deterministic = false; // switch to control if the simulation should run with a predefined random-seed
	static constexpr int seeed = 1; // seed for deterministec behaviour
	extern Random* randomengine;

	// simStep "clock" to track the age of a generation
	extern int generationAge;

	/*extern void initFolderDir();
	extern void initFolderDir(std::string baseDir);*/
}