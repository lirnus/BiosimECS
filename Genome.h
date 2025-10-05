#pragma once
#include "Neurons.h" //incl World, ecs_framework -> global_params

namespace bs {

	Entity createGenome(World* w, Entity p);

	//struct Genome {
	//	std::array<uint32_t, numberOfGenes> DNA;
	//	Color col;
	//	std::vector<NeuronTypes> topoOrder; // i could reserve NUM_NEURONS
	//	std::array<std::array<Adjacency, numberOfGenes>, num_neurons> bwd_adjacency;
	//};

	//struct Connection { bool valid = false;  NeuronTypes source; NeuronTypes sink; float weight; }; //zero-initialized as
	//struct Adjacency { bool valid = false;  NeuronTypes neighbour{}; float weight{}; }; //zero-initialized as { false, NeuronTypes{0}, 0.0f } { false, NeuronTypes{0}, NeuronTypes{0}, 0.0f }


	std::array<uint32_t, numberOfGenes> generateDNA();

	std::array<Connection, numberOfGenes> mapDNA2Connections(std::array<uint32_t, numberOfGenes>&  genes);

	uint32_t extractBits(uint32_t x, int low, int high);

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_fwdAdj(const std::array<Connection, numberOfGenes>& conn_list);

	void checkForLoops_DFS(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> &fwd_adj, 
		std::array<Connection, numberOfGenes> &conn_list);

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_bwdAdj(const std::array<Connection, numberOfGenes>& conn_list);

	void calculate_topoOrder(const std::array<std::array<Adjacency, numberOfGenes>, 
		NUM_NEURONS>& bwd_adj, std::vector<NeuronTypes>& topoOrder);

	void delete_connection(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& fwd_adj, 
		std::array<Connection, numberOfGenes> &conn_list, int source, int sink);
}