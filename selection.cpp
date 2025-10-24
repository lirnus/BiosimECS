#include "selection.h"

namespace bs {

	//lookup-table
	std::array<SelectionFunc, NUM_SELCRIT> funcTableSelCrit{};

	void initSelectionFuncTable() {
		funcTableSelCrit.fill(nullptr);

		funcTableSelCrit[NO_SELECTION] = &no_selection;
		funcTableSelCrit[KILLRIGHTHALF] = &killRightHalf;
		funcTableSelCrit[GOLEFT_GRADIENT] = &goLeft_Gradient;
	}

	// selection functions
	void no_selection(World* w) {}

	void killRightHalf(World* w) {
		// get the Pos-component of every pixie on the grid, and set their fitness to 0 if they are on the right half
		//DEBUG:
		//int deathCount{};
		int gsX_2 = worldParams->gridSizeX / 2;
		for (const Entity& entity : w->fitness.get_entities()) {

			if (w->Pos.get(entity).xPos > gsX_2) {
				//deathCount++;
				if (!w->fitness.has(entity)) { std::cerr << "joooooo hat keine fitness"; }
				w->fitness.get(entity) = 0.0;
			}
		}
		//std::cout << "deathCount: " << deathCount << "\n";
	}

	void goLeft_Gradient(World* w) {
		int gsX = worldParams->gridSizeX;
		for (const Entity& entity : w->fitness.get_entities()) {

			if (!w->fitness.has(entity)) { std::cerr << "joooooo hat keine fitness"; }
				
			w->fitness.get(entity) = static_cast<float>(gsX - w->Pos.get(entity).xPos) / gsX;
		}
	}
}