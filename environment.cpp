#include "environment.h"

namespace bs {

	std::vector<Position> barriers{};

	void initBarriers() { // to be called once at the start of the simulation. vector barriers doesn't change afterwards

		int gsX = worldParams->gridSizeX;
		int gsY = worldParams->gridSizeY;
		uint8_t bk = worldParams->Barriers_Key;

		// BARRIERS //////////////////////////
		if (bk == NO_BARRIERS) return;
		else if (bk == VERTICAL_BARRIER) {
			// vertical barrier
			std::vector<Position> vert_barr;
			vert_barr.reserve(gsY / 2);
			for (int i = worldParams->gridSizeY / 4; i < gsY * 3 / 4; i++) {
				vert_barr.push_back(Position{ i, gsX / 2 });
			}
			barriers = vert_barr;
		}
		else if (bk == HORIZONTAL_BARRIER) {
			// horizontal barrier
			std::vector<Position> hor_barr(gsX / 2);
			for (int i = gsX / 4; i < gsX * 3 / 4; i++) {
				hor_barr.push_back(Position{ gsY / 2, i });
			}
			barriers = hor_barr;
		}

	} // end initBarriers

	std::vector<Interactive> interactives{};

	void initInteractives(World* w, uint8_t key) { //to be called every generation (inside setEnvironment). Food & other interactives get distributed randomly

		int gsX = worldParams->gridSizeX;
		int gsY = worldParams->gridSizeY;

		if (key == NO_INTERACTIVES) return;
		else if (key == SPARSE_FOOD) {
			int num_food = (gsX * gsY) / 40; // 1 in 40 gridcells are food
			interactives.clear();

			for (int i = 0; i < num_food; i++) {
				int randX;
				int randY;
				while (true) {
					randX = randomengine->getRandomIntCustom(0, gsX - 1);
					randY = randomengine->getRandomIntCustom(0, gsY - 1);
					if (w->getGridCell(randY, randX) == EMPTY) { break; }
				}
				interactives.push_back(Interactive{ {randY, randX}, FOOD });
				w->setGridCell(randY, randX, FOOD);
			}
		}
		if (key == DENSE_FOOD) {
			int num_food = (gsX * gsY) / 20; // 1 in 20 gridcells are food

			for (int i = 0; i < num_food; i++) {
				int randX;
				int randY;
				while (true) {
					randX = randomengine->getRandomIntCustom(0, gsX - 1);
					randY = randomengine->getRandomIntCustom(0, gsY - 1);
					if (w->getGridCell(randY, randX) == EMPTY) { break; }
				}
				interactives.push_back(Interactive{ {randY, randX}, FOOD });
				w->setGridCell(randY, randX, FOOD);
			}
		}

	}// end void initInteractives

	void setEnvironment(World* w) {
		// place barriers in world
		for (Position& pos : barriers) {
			w->setGridCell(pos.yPos, pos.xPos, BARRIER);
		}
		// place Food/Interactives in World
		initInteractives(w, worldParams->Interactives_Key);
	}

} // end namespace bs