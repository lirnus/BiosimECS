#include "World.h"

#include <algorithm>
#include <iostream>
#include <iomanip> // for debug

namespace bs {
	World::World() : grid(worldParams->gridSizeY * worldParams->gridSizeX) {
		std::fill(grid.begin(), grid.end(), EMPTY); // initialize a world with specified size and fill it with empty
        queueForMove.reserve(worldParams->numberOfPixies);
	}

    int World::getGridCell(int r, int c) const {
        if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        return grid[r * worldParams->gridSizeX + c]; // automatic conversion of row, col in 1D
    }
    int World::getGridCell(Position& pos) const {
        int r = pos.yPos;
        int c = pos.xPos;
        if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        return grid[r * worldParams->gridSizeX + c]; // automatic conversion of row, col in 1D
    }

    void World::setGridCell(int r, int c, int value) {
        if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        grid[r * worldParams->gridSizeX + c] = value;
    }
    void World::setGridCell(std::vector<int> yxVec, int value) {
        int r = yxVec[0];
        int c = yxVec[1];
        if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        grid[r * worldParams->gridSizeX + c] = value;
    }
    void World::setGridCell(Position& pos, int value) {
        int r = pos.yPos;
        int c = pos.xPos;
        if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        grid[r * worldParams->gridSizeX + c] = value;
    }

    bool World::isInBounds(int r, int c) {
        return (
            r >= 0 &&
            r < worldParams->gridSizeY &&
            c >= 0 &&
            c < worldParams->gridSizeX);
    }
    bool World::isInBounds(Position& pos) {
        return (
            pos.yPos >= 0 &&
            pos.yPos < worldParams->gridSizeY &&
            pos.xPos >= 0 &&
            pos.xPos < worldParams->gridSizeX);
    }

	void World::printGrid() {
        std::cout << "---------------------------------\n";
        for (int r = 0; r < worldParams->gridSizeY; ++r) {
            for (int c = 0; c < worldParams->gridSizeX; ++c) {
                std::cout << grid[r * worldParams->gridSizeX + c] << " ";
                // Debug:
                /*std::cout << std::fixed << std::setprecision(1);
                if (fitness.has(grid[r * gridsizeX + c])) {
                    std::cout << static_cast<float>(fitness.get(grid[r * gridsizeX + c])) << " ";
                }*/
                //else { std::cout << grid[r * gridsizeX + c] << " "; }
            }
            std::cout << "\n";
        }
        std::cout << "---------------------------------\n";
	}
}