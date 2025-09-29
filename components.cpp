#include "components.h"

namespace bs {
	ComponentStorage<Position> Pos;

	ComponentStorage<Color> Col;

	ComponentStorage<float> facing;

	ComponentStorage<MoveUrge> move_urge;

	ComponentStorage<BrainState> brainstate;

	ComponentStorage<std::vector<NeuronFunc>> topoOrder;

	ComponentStorage<std::vector<std::vector<int>>> bwd_adjacency;
}