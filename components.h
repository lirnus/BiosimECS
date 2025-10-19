#include <cstdint>
#include <array>

#include "ecs_framework.h" // incl global_params
//#include "Genome.h"

#pragma once

namespace bs {
	static constexpr float VOID = FLT_MAX;

	// components for pixies
	struct Position { int yPos, xPos; };

	struct Color { uint8_t r, g, b; };

	struct MoveUrge { int8_t moveY, moveX; };

	struct BrainState { 
		BrainState() {
			outputs.fill(VOID);
			lastStepOutputs.fill(0.0);
		}

		std::array<float, NUM_NEURONS> outputs; 
		std::array<float, NUM_NEURONS> lastStepOutputs;
	};
	
	struct Neighbourhood {
		Neighbourhood(){
			neighbours.reserve(defaultSearchRadius * defaultSearchRadius); // pi*r² ?
			distances.reserve(defaultSearchRadius * defaultSearchRadius);
			relAngles.reserve(defaultSearchRadius * defaultSearchRadius);
		}

		bool up_to_date = false;
		std::vector<Entity> neighbours;
		std::vector<float> distances;
		std::vector<float> relAngles;
	};


	// components for genomes
	struct Connection { bool valid = false;  NeuronTypes source; NeuronTypes sink; float weight; }; //zero-initialized as
	
	struct Adjacency { bool valid = false;  NeuronTypes neighbour{}; float weight{}; }; //zero-initialized as { false, NeuronTypes{0}, 0.0f } { false, NeuronTypes{0}, NeuronTypes{0}, 0.0f }

	struct Genome {
		Genome() {
			topoOrder.reserve(NUM_NEURONS);
		}

		std::array<uint32_t, numberOfGenes> DNA;
		Color col;
		std::vector<NeuronTypes> topoOrder; // maybe it would be better to store NeuronFunc directly (avoid overhead lookup)
		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> bwd_adjacency;
	};

	struct startingGenome { // for startingPopulations with the use of readMetaGenome()
		std::array<uint32_t, numberOfGenes> DNA;
		Color col;
		int numClones;
	};
	
}