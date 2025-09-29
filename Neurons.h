
#include <cstdint>
#include "ecs_framework.h"

#pragma once

namespace bs {

	// enumerator representing different Neurons.
	// Has to be sorted: sensors, internals, actions
	enum NeuronTypes: uint8_t {
		sX_POS, // 0U
		sY_POS,
		iINT_1, // 2U
		aMOVE_W,	// 3U
		aMOVE_E,
		aMOVE_N,
		aMOVE_S,
		NUM_NEURONS // 7U
	};

	// vector tracking the indexation of the 3 neuron "classes"
	extern const std::vector<int> neuronClasses;

	// lookup-Table for Neuron functions
	/*using NeuronFunc = void(*)(Entity e);
	extern std::vector<NeuronFunc> funcTable(NUM_NEURONS);*/
	void initFuncTable();


	// Neuron functions

	void xPos_neuronfunc(Entity e);
}