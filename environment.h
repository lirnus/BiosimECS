#include "Genome.h"

#pragma once

namespace bs {

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