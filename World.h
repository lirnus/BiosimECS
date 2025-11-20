#include <vector>
#include <unordered_set>
#include <cstdint>

#include "components.h" // incl ecs_framework, global_params

#pragma once


namespace bs {

    static constexpr int EMPTY = UINT32_MAX; // int is signed - this value will wrap around and actually be -1
    static constexpr int BARRIER = UINT32_MAX - 1;
    static constexpr int FOOD = UINT32_MAX - 2;
    static constexpr int FLAG = UINT32_MAX - 3;

    // structs for different components


    class World {
        int worldSizeX;
        int worldSizeY;
        std::vector<int> grid;
        std::vector<uint8_t> pheromone_grid;
    public:
        World(const WorldParams* wp);

        EntityManager pixie_em; // an EntityManager to give away entities for pixie

        // components for pixies
        ComponentStorage<Position> Pos; // its position (y/x)
        ComponentStorage<Entity> PixieGenomes; // a reference to its genome entity
        ComponentStorage<Entity> parent; // a component tracking its predecessor from the previous generation
        ComponentStorage<float> facing; // its facing direction
        ComponentStorage<MoveUrge> move_urge; // values that indicate in which direction the pixie wants to move
        ComponentStorage<BrainState> brainstate; // the in- and outputs for its little brain
        ComponentStorage<float> fitness; // its current fitness value
        ComponentStorage<float> searchRadius; // the radius it uses to scan its environment
        ComponentStorage<float> energy; // a resource that a pixie can amount

        ComponentStorage<Neighbourhood> pixie_neighbourhood; // a cache structure to save a current snapshot of its neighbourhood

        std::unordered_set<Entity> queueForMove; // pixies that indicate that they want to move land here

        EntityManager braintemplates_em; // a separate EntityManager to give away entities for Brain-templates

        ComponentStorage<Genome> genome; // a struct containing all informations of a unique genome

        int getGridCell(int r, int c) const;
        int getGridCell(Position& pos) const;
        void setGridCell(int r, int c, int value);
        void setGridCell(std::vector<int>, int value);
        void setGridCell(Position&, int value);
        uint8_t getPheromoneCell(int r, int c) const;
        uint8_t getPheromoneCell(Position& pos) const;
        void setPheromoneCell(int r, int c, uint8_t value);
        void setPheromoneCell(Position& pos, uint8_t value);
        bool isInBounds(int r, int c) const;
        bool isInBounds(Position& pos) const;

        void pheromoneDecay(uint8_t decayAmount = 64);

        void printGrid();
        void printPheromoneGrid();
    };
}
