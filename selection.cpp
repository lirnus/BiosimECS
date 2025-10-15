#include "selection.h"

namespace bs {

	//lookup-table
	std::array<SelectionFunc, NUM_SELCRIT> funcTableSelCrit{};

	void initSelectionFuncTable() {
		funcTableSelCrit[KILLRIGHTHALF] = &killRightHalf;
	}

	// selection functions
	void killRightHalf(World* w) {
		// get the Pos-component of every pixie on the grid, and set their fitness to 0 if they are on the right half
		//DEBUG:
		//int deathCount{};

		for (const Entity& entity : w->fitness.get_entities()) {

			if (w->Pos.get(entity).xPos > gridsizeX / 2) {
				//deathCount++;
				if (!w->fitness.has(entity)) { std::cerr << "joooooo hat keine fitness"; }
				w->fitness.get(entity) = 0.0;
			}
		}
		//std::cout << "deathCount: " << deathCount << "\n";
	}
}