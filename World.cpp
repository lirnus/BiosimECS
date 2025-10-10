#include "World.h"

#include <algorithm>
#include <iostream>

namespace bs {
	World::World() : grid(gridsizeY* gridsizeX) {
		std::fill(grid.begin(), grid.end(), EMPTY); // initialize a world with specified size and fill it with empty
        queueForMove.reserve(numberOfPixies);
	}

    int World::getGridCell(int r, int c) {
        if (r >= gridsizeY || c >= gridsizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        return grid[r * gridsizeX + c]; // automatic conversion of row, col in 1D
    }
    int World::getGridCell(Position& pos) {
        int r = pos.yPos;
        int c = pos.xPos;
        if (r >= gridsizeY || c >= gridsizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        return grid[r * gridsizeX + c]; // automatic conversion of row, col in 1D
    }

    void World::setGridCell(int r, int c, int value) {
        if (r >= gridsizeY || c >= gridsizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        grid[r * gridsizeX + c] = value;
    }
    void World::setGridCell(std::vector<int> yxVec, int value) {
        int r = yxVec[0];
        int c = yxVec[1];
        if (r >= gridsizeY || c >= gridsizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        grid[r * gridsizeX + c] = value;
    }
    void World::setGridCell(Position& pos, int value) {
        int r = pos.yPos;
        int c = pos.xPos;
        if (r >= gridsizeY || c >= gridsizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        grid[r * gridsizeX + c] = value;
    }

    bool World::isInBounds(int r, int c) {
        return (
            r >= 0 &&
            r < gridsizeY &&
            c >= 0 &&
            c < gridsizeY);
    }
    bool World::isInBounds(Position& pos) {
        return (
            pos.yPos >= 0 &&
            pos.yPos < gridsizeY &&
            pos.xPos >= 0 &&
            pos.xPos < gridsizeY);
    }

	void World::printGrid() {
        std::cout << "---------------------------------\n";
        for (int r = 0; r < gridsizeY; ++r) {
            for (int c = 0; c < gridsizeX; ++c) {
                std::cout << grid[r * gridsizeX + c] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "---------------------------------\n";
	}
}