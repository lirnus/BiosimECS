#include "pixie_funcs.h"

namespace bs {


	void executeMove(World* w, const Entity& p) {
		// translate the moveUrges of this pixie into movement on the grid.
		// check before if movement is blocked
		// and reset moveUrges afterwards.

		// clamp move urges to min -1, max 1 in each dimension
		uint8_t moveX = std::clamp(static_cast<int>(w->move_urge.get(p).moveX), -1, 1);
		uint8_t moveY = std::clamp(static_cast<int>(w->move_urge.get(p).moveY), -1, 1);

		// calculate destination
		int dest_X = std::clamp(w->Pos.get(p).xPos + moveX, 0, gridsizeX-1);
		int dest_Y = std::clamp(w->Pos.get(p).yPos + moveY, 0, gridsizeY-1);

		// check if free
		if (const auto cell = w->getGridCell(dest_Y, dest_X); // C++17 format
			cell != EMPTY && cell != BARRIER && cell != FOOD) {
			// clear old position on grid
			w->setGridCell(w->Pos.get(p), EMPTY);
			// update Position
			w->Pos.get(p).xPos = dest_X;
			w->Pos.get(p).yPos = dest_Y;
			// move Pixie to new position
			w->setGridCell(w->Pos.get(p), p);
		}

		// reset moveUrge
		w->move_urge.get(p).moveX = 0;
		w->move_urge.get(p).moveY = 0;
	}

}