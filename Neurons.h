
#include <cstdint>
#include "pixie_funcs.h" // incl World, ecs_framework, components, global_params

#pragma once

namespace bs {
	// file containing Neuron functions

	// "axon" functions to propagate information through the brain network
	void sumAndClamp(const std::array<std::array<Adjacency, MAX_ADJ>, MAX_NEURONS>& bwd_adj, const int& neuron, World* w, Entity p); // uses bwd_adj to find all sources, sum up those values and clamp them with tanh()

	void update_BrainState(const int& neuron, World* w, Entity p); // function to retrieve outputs from one neuronfunc and store it in the appropriate brainstate slot

	void evaluate(const std::array<std::array<Adjacency, MAX_ADJ>, MAX_NEURONS>& bwd_adj, const int& neuron, World* w, Entity p);

	// neuron functions (directly return their output)
	//s
	float xPos_neuronfunc(World* w, Entity p);

	float yPos_neuronfunc(World* w, Entity p);

	float popDensityFwd_neuronfunc(World* w, Entity p);

	float age_neuronfunc(World* w, Entity p);

	float onOff_neuronfunc(World* w, Entity p);


	//a													actions return floats, but that can be any arbitray number 
	float moveW_neuronfunc(World* w, Entity p);

	float moveE_neuronfunc(World* w, Entity p);

	float moveN_neuronfunc(World* w, Entity p);

	float moveS_neuronfunc(World* w, Entity p);

	float emitPheromone_neuronfunc(World* w, Entity p);

	//void _neuronfunc(World* w, Entity p);


	// brain functions
	void execute_staticBrain(World* w, Entity p);

	// lookup-Table for Neuron functions
	using NeuronFunc = float(*)(World* w, Entity e);
	extern std::vector<NeuronFunc> funcTable;
	void initNeuronFuncTable();
}