#include <cstdint>

#include "global_params.h"
#include "ecs_framework.h"
#include "Neurons.h"

#pragma once

namespace bs {
	static constexpr float VOID = FLT_MAX;

	// components for pixies
	struct Position { int yPos, xPos; };
	extern ComponentStorage<Position> Pos;

	struct Color { uint8_t r, g, b; };
	extern ComponentStorage<Color> Col;

	extern ComponentStorage<float> facing;

	struct MoveUrge { uint8_t moveY, moveX; };
	extern ComponentStorage<MoveUrge> move_urge;

	struct BrainState { std::vector<float> outputs{ NUM_NEURONS, VOID }, lastStepOutputs{ NUM_NEURONS, 0.0 }; };
	extern ComponentStorage<BrainState> brainstate;

	extern ComponentStorage<float> fitness;



	// components for genomes
	struct Genome { std::vector<uint32_t> DNA; };
	extern ComponentStorage<Genome> genome;

	using NeuronFunc = void(*)(Entity e);
	extern ComponentStorage<std::vector<NeuronFunc>> topoOrder;

	extern ComponentStorage<std::vector<std::vector<int>>> bwd_adjacency;


}