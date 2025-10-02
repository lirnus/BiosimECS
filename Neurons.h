
#include <cstdint>
#include "ecs_framework.h"
#include "World.h"

#pragma once

namespace bs {

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
	extern const std::vector<int> neuronClasses;

	void xPos_neuronfunc(World* w, Entity e);

	// lookup-Table for Neuron functions
	/*using NeuronFunc = void(*)(Entity e);
	extern std::vector<NeuronFunc> funcTable(NUM_NEURONS);*/
	void initFuncTable();
}