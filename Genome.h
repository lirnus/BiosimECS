#pragma once
#include "World.h"
#include "Neurons.h"

namespace bs {

	Entity createGenome(World* w, Entity p);

	struct Connection { bool valid = false;  NeuronTypes source; NeuronTypes sink; float weight; }; //zero-initialized as { false, NeuronTypes{0}, NeuronTypes{0}, 0.0f }

	struct Adjacency { bool valid = false;  NeuronTypes neighbour{}; float weight{}; }; //zero-initialized as { false, NeuronTypes{0}, 0.0f }

	std::array<uint32_t, numberOfGenes> generateDNA();

	std::array<Connection, numberOfGenes> mapDNA2Connections(std::array<uint32_t, numberOfGenes>&  genes);

	uint32_t extractBits(uint32_t x, int low, int high);

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_fwdAdj(std::array<Connection, numberOfGenes>& conn_list);

	void checkForLoops_DFS(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> &fwd_adj, 
		std::array<Connection, numberOfGenes> &conn_list);

	void delete_connection(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& fwd_adj, 
		std::array<Connection, numberOfGenes> &conn_list, int source, int sink);
}