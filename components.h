#include <cstdint>

#include "global_params.h"
#include "ecs_framework.h"
#include "Neurons.h"

#pragma once

namespace bs {

	// components for pixies
	struct Position { int yPos, xPos; };
	extern ComponentStorage<Position> Pos;

	struct Color { uint8_t r, g, b; };
	extern ComponentStorage<Color> Col;

	extern ComponentStorage<float> facing;

	struct MoveUrge { uint8_t moveY, moveX; };
	extern ComponentStorage<MoveUrge> move_urge;

	struct BrainState { float outputs[NUM_NEURONS], lastStepOutputs[NUM_NEURONS]; };
	extern ComponentStorage<BrainState> brainstate;



	// components for genomes
	using NeuronFunc = void(*)(Entity e);
	extern ComponentStorage<std::vector<NeuronFunc>> topoOrder;

	extern ComponentStorage<std::vector<std::vector<int>>> bwd_adjacency;


}