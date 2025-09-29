#include "Neurons.h"
#include "components.h"
#include <cassert>

namespace bs {

	// lookup table for Neuron functions
	const std::vector<int> neuronClasses = { 0, 2, 3, 7 };
	using NeuronFunc = void(*)(Entity e);
	std::vector<NeuronFunc> funcTable(NUM_NEURONS);

	void initFuncTable() {
		funcTable[sX_POS] = &xPos_neuronfunc;
	}


	void xPos_neuronfunc(Entity e) {
		// get the x position of the pixie
		assert(Pos.has(e));
		int x_pos = Pos.get(e).xPos;

		// divide it by the total x-size of the grid
		float out = static_cast<float>(gridsizeX) / x_pos;

		// transfer the result into the outputs-list of that pixie's brain
		brainstate.get(e).outputs[sX_POS] = out;
	}
}