#include "World.h"

#include <algorithm>
#include <iostream>
#include <iomanip> // for debug

namespace bs {
	World::World(const WorldParams* wp) : 
        worldSizeX(wp->gridSizeX),
        worldSizeY(wp->gridSizeY),
        grid(worldSizeY * worldSizeX), 
        pheromone_grid(worldSizeY * worldSizeX)
    {
		std::fill(grid.begin(), grid.end(), EMPTY); // initialize a world with specified size and fill it with empty
        queueForMove.reserve(worldParams->numberOfPixies);
	}

    int World::getGridCell(int r, int c) const {
        //if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        if (isInBounds(r, c)) {
            return grid[r * worldSizeX + c]; // automatic conversion of row, col in 1D
        }
        else throw std::out_of_range("Index oor");
    }
    int World::getGridCell(Position& pos) const {
        int r = pos.yPos;
        int c = pos.xPos;
        if (isInBounds(r, c)) {
            return grid[r * worldSizeX + c]; // automatic conversion of row, col in 1D
        }
        else throw std::out_of_range("Index oor");
    }

    void World::setGridCell(int r, int c, int value) {
        //if (r >= worldParams->gridSizeY || c >= worldParams->gridSizeX || r < 0 || c < 0) throw std::out_of_range("Index oor");
        if (isInBounds(r, c)) {
            grid[r * worldSizeX + c] = value;
        }
        else throw std::out_of_range("Index oor");
    }
    void World::setGridCell(std::vector<int> yxVec, int value) {
        int r = yxVec[0];
        int c = yxVec[1];
        if (isInBounds(r, c)) {
            grid[r * worldSizeX + c] = value;
        }
        else throw std::out_of_range("Index oor");
    }
    void World::setGridCell(Position& pos, int value) {
        int r = pos.yPos;
        int c = pos.xPos;
        if (isInBounds(r, c)) {
            grid[r * worldSizeX + c] = value;
        }
        else throw std::out_of_range("Index oor");
    }

    uint8_t World::getPheromoneCell(int r, int c) const {
        if (isInBounds(r, c)) {
            return pheromone_grid[r * worldSizeX + c]; // automatic conversion of row, col in 1D
        }
        else throw std::out_of_range("Index oor");
    }
    uint8_t World::getPheromoneCell(Position& pos) const {
        int r = pos.yPos;
        int c = pos.xPos;
        if (isInBounds(r, c)) {
            return pheromone_grid[r * worldSizeX + c]; // automatic conversion of row, col in 1D
        }
        else throw std::out_of_range("Index oor");
    }
    void World::setPheromoneCell(int r, int c, uint8_t value) {
        if (isInBounds(r, c)) {
            pheromone_grid[r * worldSizeX + c] += value;
        }
        else throw std::out_of_range("Index oor");
    }
    void World::setPheromoneCell(Position& pos, uint8_t value) {
        int r = pos.yPos;
        int c = pos.xPos;
        if (isInBounds(r, c)) {
            pheromone_grid[r * worldSizeX + c] += value;
        }
        else throw std::out_of_range("Index oor");
    }

    bool World::isInBounds(int r, int c) const {
        return (
            r >= 0 &&
            r < worldSizeY &&
            c >= 0 &&
            c < worldSizeX);
    }
    bool World::isInBounds(Position& pos) const {
        return (
            pos.yPos >= 0 &&
            pos.yPos < worldSizeY &&
            pos.xPos >= 0 &&
            pos.xPos < worldSizeX);
    }

    void World::pheromoneDecay(uint8_t decayAmount) {
        for (uint8_t& cell : pheromone_grid) {
            // remove 25% of max intensity (64) from all values
            cell = (cell > decayAmount) ? (cell - decayAmount) : 0;
        }
    }

	void World::printGrid() {
        std::cout << "---------------------------------\n";
        for (int r = 0; r < worldSizeY; ++r) {
            for (int c = 0; c < worldSizeX; ++c) {
                std::cout << grid[r * worldSizeX + c] << " ";
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
    void World::printPheromoneGrid() {
        std::cout << "+++++++++++++++++++++++++++++++++\n";
        for (int r = 0; r < worldSizeY; ++r) {
            for (int c = 0; c < worldSizeX; ++c) {
                std::cout << static_cast<int>(pheromone_grid[r * worldSizeX + c]) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "+++++++++++++++++++++++++++++++++\n";
    }
}