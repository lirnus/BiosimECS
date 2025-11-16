//#include "ecs_framework.h"
//#include "World.h"
#include "analysis.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	void initializePixie(World* w, Entity& newPixie);

	void spawnPixie(World* w);
	void inheritPixie(World* newW, const Genome& oldGenome);
	void inheritPixie(World* newW, const startingGenome& strt_gnm);
	

	void newGeneration(World* newW); // generate genomes from scratch
	void newGeneration(World* newW, const std::vector<Genome>& genome); // for succeeding generenations
	void newGeneration_fromTextfile(World* newW);

	
	void eachSimStep(World*, int gen, int age);
	void evaluateFitness(World* w);
	std::vector<Genome> select(World*);


	void simulateGenerations();
}