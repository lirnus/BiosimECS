#pragma once
#include "Neurons.h" //incl World, ecs_framework -> global_params

namespace bs {

	Entity createGenome(World* w, Entity p);
	void initializeBrain(World* w, Entity& newGenome);

	std::array<uint32_t, numberOfGenes> mutateDNA(const Genome& gnome);
	Color generateSimilarColor(const Color& old_c, float factor);
	Entity inheritGenome(World* w, Entity p, const Genome& old_gnm);
	Entity inheritGenome_Missense(World* w, Entity& p, const std::array<uint32_t, numberOfGenes>& new_dna, const Genome& old_gnm);
	Entity inheritGenome_Weight(World* w, Entity& p, std::array<uint32_t, numberOfGenes>& new_dna, const Genome& old_gnm);
	Entity inheritGenome_Silent(World* w, Entity& p, const std::array<uint32_t, numberOfGenes>& new_dna, const Genome& old_gnm);
	Entity inheritGenome_fromDNA(World* w, Entity& p, const startingGenome& strt_gnm);

	std::array<uint32_t, numberOfGenes> generateDNA();

	Entity checkForClone(World* w, std::array<uint32_t, numberOfGenes> dna);

	std::array<Connection, numberOfGenes> mapDNA2Connections(std::array<uint32_t, numberOfGenes>&  genes);
	Connection mapDNA2Connection_single(const uint32_t& gene);
	uint32_t extractBits(uint32_t x, int low, int high);
	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_fwdAdj(const std::array<Connection, numberOfGenes>& conn_list);
	void checkForLoops_DFS(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& fwd_adj,
		std::array<Connection, numberOfGenes>& conn_list);
	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_bwdAdj(const std::array<Connection, numberOfGenes>& conn_list);
	void calculate_topoOrder(const std::array<std::array<Adjacency, numberOfGenes>, 
		NUM_NEURONS>& bwd_adj, std::vector<NeuronTypes>& topoOrder);
	void delete_connection(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& fwd_adj, 
		std::array<Connection, numberOfGenes> &conn_list, int source, int sink);
}