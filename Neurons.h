
#include <cstdint>
#include "World.h" // incl ecs_framework, components, global_params

#pragma once

namespace bs {

	

	// vector tracking the indexation of the 3 neuron "classes"
	extern const std::vector<int> neuronClasses;

	void xPos_neuronfunc(World* w, Entity e);

	// lookup-Table for Neuron functions
	/*using NeuronFunc = void(*)(Entity e);
	extern std::vector<NeuronFunc> funcTable(NUM_NEURONS);*/
	void initFuncTable();
}