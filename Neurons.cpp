#include "Neurons.h"
#include "World.h"
#include "components.h"
#include <cassert>


namespace bs {

	// lookup table for Neuron functions
	const std::vector<int> neuronClasses = { 0, 4, 7, 11 };

	//using NeuronFunc = void(*)(World* w, Entity e);
	std::vector<NeuronFunc> funcTable(NUM_NEURONS);

	void initFuncTable() {
		funcTable[sX_POS] = &xPos_neuronfunc;
	}

	void xPos_neuronfunc(World* w, Entity e) {
		// get the x position of the pixie
		assert(w->Pos.has(e));
		int x_pos = w->Pos.get(e).xPos;

		// divide it by the total x-size of the grid
		float out = static_cast<float>(gridsizeX) / x_pos;

		// transfer the result into the outputs-list of that pixie's brain
		w->brainstate.get(e).outputs[sX_POS] = out;
	}
}