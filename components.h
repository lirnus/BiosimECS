#include <cstdint>

#include "global_params.h"
#include "ecs_framework.h"

#pragma once

namespace bs {
	static constexpr int num_neurons = 11; // THIS VALUE NEEDS TO BE THE SAME AS NUM_NEURONS IN Neurons.h, can't reference it here because of circular includes
	static constexpr float VOID = FLT_MAX;

	class World;

	// components for pixies
	struct Position { int yPos, xPos; };

	struct Color { uint8_t r, g, b; };

	struct MoveUrge { uint8_t moveY, moveX; };

	struct BrainState { std::vector<float> outputs{ num_neurons, VOID }, lastStepOutputs{ num_neurons, 0.0 }; };


	using NeuronFunc = void(*)(World* w, Entity e);
	// components for genomes
	struct Genome { std::vector<uint32_t> DNA; 
					Color col; 
					std::vector<NeuronFunc> topoOrder;
					std::vector<std::vector<int>> bwd_adjacency;
	};


	

}