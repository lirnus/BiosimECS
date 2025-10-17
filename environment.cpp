#include "environment.h"

namespace bs {

	std::vector<Position> barriers{};

	void initBarriers() { // to be called once at the start of the simulation. vector barriers doesn't change afterwards

		// BARRIERS //////////////////////////
		if (Barriers_Key == NO_BARRIERS) return;
		else if (Barriers_Key == VERTICAL_BARRIER) {
			// vertical barrier
			std::vector<Position> vert_barr;
			vert_barr.reserve(gridsizeY / 2);
			for (int i = gridsizeY / 4; i < gridsizeY * 3 / 4; i++) {
				vert_barr.push_back(Position{ i, gridsizeX / 2 });
			}
			barriers = vert_barr;
		}
		else if (Barriers_Key == HORIZONTAL_BARRIER) {
			// horizontal barrier
			std::vector<Position> hor_barr(gridsizeX / 2);
			for (int i = gridsizeX / 4; i < gridsizeX * 3 / 4; i++) {
				hor_barr.push_back(Position{ gridsizeY / 2, i });
			}
			barriers = hor_barr;
		}

	} // end initBarriers

	std::vector<Interactive> interactives{};

	void initInteractives(World* w, uint8_t key) { //to be called every generation (inside setEnvironment). Food & other interactives get distributed randomly

		if (key == NO_INTERACTIVES) return;
		else if (key == SPARSE_FOOD) {
			int num_food = (gridsizeX * gridsizeY) / 40; // 1 in 40 gridcells are food
			interactives.clear();

			for (int i = 0; i < num_food; i++) {
				int randX;
				int randY;
				while (true) {
					randX = randomengine->getRandomIntCustom(0, gridsizeX - 1);
					randY = randomengine->getRandomIntCustom(0, gridsizeY - 1);
					if (w->getGridCell(randY, randX) == EMPTY) { break; }
				}
				interactives.push_back(Interactive{ {randY, randX}, FOOD });
				w->setGridCell(randY, randX, FOOD);
			}
		}
		if (key == DENSE_FOOD) {
			int num_food = (gridsizeX * gridsizeY) / 20; // 1 in 20 gridcells are food

			for (int i = 0; i < num_food; i++) {
				int randX;
				int randY;
				while (true) {
					randX = randomengine->getRandomIntCustom(0, gridsizeX - 1);
					randY = randomengine->getRandomIntCustom(0, gridsizeY - 1);
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
		initInteractives(w, Interactives_Key);
	}

} // end namespace bs