#include <cstdint>
#include <array>

#include "ecs_framework.h" // incl global_params
//#include "Genome.h"

#pragma once

namespace bs {
	//static constexpr int num_neurons = 11; // THIS VALUE NEEDS TO BE THE SAME AS NUM_NEURONS IN Neurons.h, can't reference it here because of circular includes
	static constexpr float VOID = FLT_MAX;

	// enumerator representing different Neurons.
	// Has to be sorted: sensors, internals, actions
	enum NeuronTypes : uint8_t {
		sX_POS, // 0U
		sY_POS,
		sPOP_DENSITY_FWD,
		sAGE,
		iINT_1, // 4U
		iINT_2,
		iINT_3,
		aMOVE_W,	// 7U
		aMOVE_E,
		aMOVE_N,
		aMOVE_S,
		NUM_NEURONS // 11U
	};

	// vector tracking the indexation of the 3 neuron "classes"
	//extern const std::vector<int> neuronClasses;
	//const std::vector<int> neuronClasses = { 0, 4, 7, 11 };
	static constexpr std::array<int, 4> neuronClasses = { 0, 4, 7, 11 };

	//class World;

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

	
}