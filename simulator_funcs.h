//#include "ecs_framework.h"
//#include "World.h"
#include "analysis.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	struct selectedGenome {// a POD struct to contain different short-lived variables when inheriting Pixies
		Entity genomeID;
		Entity parentID;
	};

	void initializePixie(World* w, Entity& newPixie);

	void spawnPixie(World* w);
	void inheritPixie(World* newW, const Genome& oldGenome, const Entity& parent);
	void inheritPixie(World* newW, const startingGenome& strt_gnm);
	

	void newGeneration(World* newW); // generate genomes from scratch
	void newGeneration(World* newW, const ComponentStorage<Genome>& prevMetagenome, const std::vector<Entity>& selectedGenomes); // for succeeding generenations
	void newGeneration_fromTextfile(World* newW);

	
	void eachSimStep(World*, int gen, int age);
	void evaluateFitness(World* w);
	std::vector<selectedGenome> select(World*);


	void simulateGenerations();
}