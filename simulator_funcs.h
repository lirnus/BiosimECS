//#include "ecs_framework.h"
//#include "World.h"
#include "analysis.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	struct selectedGenome {// a POD struct to contain different short-lived variables when inheriting Pixies

		/*selectedGenome(std::array<Entity, MAX_PARENTS> pIDs, 
			std::array<Entity, MAX_PARENTS> gIDs,
			std::vector<int> ps) : parentIDs{pIDs}, genomeIDs{gIDs}, parents{ps}
		{
			parents.reserve(worldParams->numberOfGenes);
		}*/

		//std::array<Entity, MAX_PARENTS> parentIDs{ INVALID }; // Entity IDs of all parent pixies
		std::array<Entity, MAX_PARENTS> genomeIDs{ INVALID }; // The corresponding genome Entity IDs of those parents

		std::vector<int> parents; // a vector mapping the indexes of parentIDs to the genes that get inherited
	};

	void initializePixie(World* w, Entity& newPixie);

	void spawnPixie(World* w);
	void inheritPixie(World* newW, const Genome& oldGenome);
	void inheritPixie(World* newW, const startingGenome& strt_gnm);


	Genome recombineGenomes(const ComponentStorage<Genome>& precMetagenome, const selectedGenome&); // for sexual reproduction
	

	void newGeneration(World* newW); // generate genomes from scratch
	void newGeneration(World* newW, const ComponentStorage<Genome>& prevMetagenome, const std::vector<Entity>& selectedGenomes); // for succeeding generenations
	void newGeneration_fromTextfile(World* newW);

	
	void eachSimStep(World*, int gen, int age);
	void evaluateFitness(World* w);
	std::vector<selectedGenome> select(World*);


	void simulateGenerations();
}