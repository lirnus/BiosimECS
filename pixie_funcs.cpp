#include "pixie_funcs.h"
#include <iostream>

namespace bs {


	void executeMove(World* w, const Entity& p) {
		// translate the moveUrges of this pixie into movement on the grid.
		
		/*std::cout << "pixie " << p << " moveX " << static_cast<int>(w->move_urge.get(p).moveX) << "\n";
		std::cout << "pixie " << p << " moveY " << static_cast<int>(w->move_urge.get(p).moveY) << "\n";*/


		// clamp move urges to min -1, max 1 in each dimension
		uint8_t moveX = std::clamp(static_cast<int>(w->move_urge.get(p).moveX), -1, 1);
		uint8_t moveY = std::clamp(static_cast<int>(w->move_urge.get(p).moveY), -1, 1);

		// calculate destination
		int dest_X = std::clamp(w->Pos.get(p).xPos + moveX, 0, gridsizeX-1);
		int dest_Y = std::clamp(w->Pos.get(p).yPos + moveY, 0, gridsizeY-1);

		// check if free
		if (w->getGridCell(dest_Y, dest_X) == EMPTY) {
			// clear old position on grid
			w->setGridCell(w->Pos.get(p), EMPTY);
			// update Position
			w->Pos.get(p).xPos = dest_X;
			w->Pos.get(p).yPos = dest_Y;
			// move Pixie to new position
			w->setGridCell(w->Pos.get(p), p);
			std::cout << "pixie " << p << " moved to " << dest_Y << ", " << dest_X << "\n";
		}

		// reset moveUrge
		w->move_urge.get(p).moveX = 0;
		w->move_urge.get(p).moveY = 0;
	}

}