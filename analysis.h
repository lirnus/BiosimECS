#include "selection.h"

#pragma once

namespace bs {

	// metadata
	void saveMetaData();
	std::string_view selCrit_printable();
	std::string_view barriers_printable();
	std::string_view interactives_printable();
	std::string_view neurons_printable(uint8_t neuron);

	/*void calculateDiversity(World* w);
	void calculateSurvivalrate(World* w);
	void writeSurvivalDiversityFile();
	void plotSurvivalDiversity();*/

	// mullerplot
	/*extern std::vector<std::map<std::vector<uint32_t>, int>> mullerplot_maps;
	extern std::map<std::vector<uint32_t>, std::vector<uint8_t>> colors_map;
	void countLineages(World* w);
	void writeLineageCounts();
	void mullerplot();*/

	// GIF
	bool shouldCreateGIF(int gen);
	void writeGIFdata(const int gen, const World* w);
	void renderGIFs();

	// save Metageonome
	void saveMetagenome(World* w);
	const std::vector<startingGenome> readMetagenome();


	// wrapper function
	//void analysis();
}