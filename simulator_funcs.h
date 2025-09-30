#include "ecs_framework.h"
#include "World.h"
#include <vector>
#include <cstdint>
#include <optional>

#pragma once

namespace bs {

	void spawnPixie(std::unique_ptr<World>& w);
	void spawnPixie(World* w, Entity genomeID);
	void spawnPixie(World* w, Entity genomeID, std::vector<uint8_t> new_rgb);

	void inheritPixie(World& newW, Entity genomeID);


	void eachSimStep(std::unique_ptr<World>&, int gen);

	//void newGeneration();
	void newGeneration(std::unique_ptr<World>& newW, const std::optional<ComponentStorage<Genome>>& genome); // for existing genomes
	void newGeneration(std::unique_ptr<World>& newW, std::unique_ptr<World>& oldW); // for succeeding generenations

	void evaluateFitness(std::unique_ptr<World>& w);

	void simulateGenerations(const std::optional<ComponentStorage<Genome>>& startingMetagenome = std::nullopt);
}