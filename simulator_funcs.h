#include "ecs_framework.h"
//#include "World.h"
#include "Genome.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	void spawnPixie(World* w);

	void inheritPixie(World* newW, const Genome oldGenome);
	



	void eachSimStep(World*, int gen);

	//void newGeneration();
	void newGeneration(World* newW);
	void newGeneration(World* newW, const std::vector<Genome>& genome); // for succeeding generenations
	std::vector<Genome> select(World*);

	void evaluateFitness(World* w);

	void simulateGenerations();
	void simulateGenerations(const std::vector<Genome>& startingMetagenome);
}