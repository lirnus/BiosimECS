#include "ecs_framework.h"
#include "World.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	void spawnPixie(World* w);
	void spawnPixie(World* w, Entity genomeID);
	void spawnPixie(World* w, Entity genomeID, std::vector<uint8_t> new_rgb);

	void inheritPixie(World& newW, Entity genomeID);


	void eachSimStep(World*, int gen);

	//void newGeneration();
	void newGeneration(World* newW, const std::optional<ComponentStorage<Genome>>& genome);
	void newGeneration(World* newW, const ComponentStorage<Genome>& genome); // for succeeding generenations

	void evaluateFitness(World* w);

	void simulateGenerations(const std::optional<ComponentStorage<Genome>>& startingMetagenome = std::nullopt);
}