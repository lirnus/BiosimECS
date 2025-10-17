//#include "ecs_framework.h"
//#include "World.h"
#include "analysis.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	void spawnPixie(World* w);
	void inheritPixie(World* newW, const Genome oldGenome);
	

	void newGeneration(World* newW); // generate genomes from scratch
	void newGeneration(World* newW, const std::vector<Genome>& genome); // for succeeding generenations
	void eachSimStep(World*, int gen, int age);
	void evaluateFitness(World* w);
	std::vector<Genome> select(World*);


	void simulateGenerations();
	void simulateGenerations(const std::vector<Genome>& startingMetagenome);
}