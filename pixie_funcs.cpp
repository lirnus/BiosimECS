#include "pixie_funcs.h"
#include <iostream>

namespace bs {

	// actions
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

	// surroundings
	Neighbourhood scanNeighbourhood(World* w, const Entity& p) {

		//first check if that Pixie already has an assigned neighbourhood
		if (w->pixie_neighbourhood.has(p)) {
			return w->pixie_neighbourhood.get(p);
		}
		//else compute from scratch
		else {
			Neighbourhood nghbhd;

			int yPos = w->Pos.get(p).yPos;
			int xPos = w->Pos.get(p).xPos;
			float searchR = w->searchRadius.get(p);

			// for each gridcell in searchRadius, look for non-empty cell and if found add to neighbours, compute distance and add to distances
			for (int r = yPos - searchR; r < yPos + searchR; r++) {
				for (int c = xPos - searchR; c < xPos + searchR; c++) {

					if (isInSearchRadius(c, r, w, p)) {
						Entity neighbour = w->getGridCell(r, c);
						if (neighbour != EMPTY) {
							nghbhd.neighbours.push_back(neighbour);
							nghbhd.distances.push_back(calcEuclidianDistance(c, r));
						}
					}
				}
			}
			w->pixie_neighbourhood.add(p, nghbhd);
			return w->pixie_neighbourhood.get(p);
		} // end else compute

	} // end scanNeighbourhood

	// math
	const Position& getRelativePosition(const int r, const int c, const Position& ownPos) {
		
		int dy = r - ownPos.yPos;
		int dx = c - ownPos.xPos;

		Position relPos{ dy, dx };
		
		return relPos;
	}

	float calcEuclidianDistance(const int x, const int y) {
		return std::sqrt(y * y + x * x);
	}
	float calcEuclidianDistance(const Position& relPos) {
		return std::sqrt(relPos.yPos * relPos.yPos + relPos.xPos * relPos.xPos);
	}

	bool isInSearchRadius(const int x, const int y, const World* w, const Entity& p) {
		return (std::sqrt(y * y + x * x) <  w->searchRadius.get(p));
	}
	bool isInSearchRadius(const Position& relPos, const World* w, const Entity& p) {
		return (std::sqrt(relPos.yPos * relPos.yPos + relPos.xPos * relPos.xPos) < w->searchRadius.get(p));
	}
}