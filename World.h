#include <vector>
#include <cstdint>

#include "global_params.h"
#include "components.h"

#pragma once


namespace bs {

    static constexpr uint32_t EMPTY = UINT32_MAX - 1;
    static constexpr uint32_t BARRIER = UINT32_MAX - 2;
    static constexpr uint32_t FOOD = UINT32_MAX - 3;

    class World {
        std::vector<uint32_t> grid;
    public:
        World();

        int getGridCell(int r, int c);
        int getGridCell(std::vector<int>);
        void setGridCell(int r, int c, uint32_t value);
        void setGridCell(std::vector<int>, uint32_t value);
        void setGridCell(Position&, uint32_t value);

        void printGrid();
    };
}
