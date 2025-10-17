#include "Genome.h"

#pragma once

namespace bs {
	enum Barriers : uint8_t {
		NO_BARRIERS,
		VERTICAL_BARRIER,
		HORIZONTAL_BARRIER
	};

	enum Interactives : uint8_t {
		NO_INTERACTIVES,
		SPARSE_FOOD,
		DENSE_FOOD,
		FOUR_FLAGS
	};

	struct Interactive {
		Position Position;
		int type;
	};

	extern std::vector<Position> barriers; // a vector containing all the positions where a Barrier should sit
	extern std::vector<Interactive> interactives; // a vector containg the positions of special interactive objects on the grid
	void initBarriers();
	void initInteractives(World* w, uint8_t key);

	void setEnvironment(World* w);
}