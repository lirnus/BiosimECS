#include <vector>
#include <cstdint>

#include "components.h" // incl ecs_framework, global_params

#pragma once


namespace bs {

    static constexpr int EMPTY = UINT32_MAX; // int is signed - this value will wrap around and actually be -1
    static constexpr int BARRIER = UINT32_MAX - 1;
    static constexpr int FOOD = UINT32_MAX - 2;

    // structs for different components


    class World {
        std::vector<int> grid;
    public:
        World();

        EntityManager pixie_em;

        ComponentStorage<Position> Pos;
        ComponentStorage<Entity> Genome_entities; 
        ComponentStorage<float> facing;
        ComponentStorage<MoveUrge> move_urge;
        ComponentStorage<BrainState> brainstate;
        ComponentStorage<float> fitness;


        EntityManager braintemplates_em;

        ComponentStorage<Genome> genome;

        int getGridCell(int r, int c);
        int getGridCell(std::vector<int>);
        void setGridCell(int r, int c, int value);
        void setGridCell(std::vector<int>, int value);
        void setGridCell(Position&, int value);

        void printGrid();
    };
}
