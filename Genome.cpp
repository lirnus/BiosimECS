#include "Genome.h"
#include <array>
#include <cmath>
#include <iostream>

namespace bs {

	Entity createGenome(World* w, Entity p) {
		// create new Genome entity
		Entity newGenome = w->braintemplates_em.create();

		// generate DNA from scratch
		std::array<uint32_t, numberOfGenes> genes = generateDNA();

		// map DNA2NeuronTypes
		// generate connection list
		std::array<Connection, numberOfGenes> conn_list = mapDNA2Connections(genes);

		// generate fwd_adjacency list
		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adjacency = generate_fwdAdj(conn_list);

		// check for loops and dead ends, remove
		checkForLoops_DFS(fwd_adjacency, conn_list);

		// generate bwd_adjacency list

		// calculate topology
		
		// save genome to comp
		w->genome.add(newGenome, {}); // either copy all the elements, or move() if already allocated on heap

		//return Genome entity
		return newGenome;
	}

	std::array<uint32_t,numberOfGenes> generateDNA() {
		std::array<uint32_t, numberOfGenes> genes_list{};

		for (int i = 0; i < numberOfGenes; i++) {
			genes_list.at(i)= randomengine->getRandom32b();
		}

		return genes_list;
	}

	std::array<Connection, numberOfGenes> mapDNA2Connections(std::array<uint32_t, numberOfGenes>  genes) {

		std::array<Connection, numberOfGenes> conn_list{};

		for (int i = 0; i < numberOfGenes; i++) {
			
			// slice the uint32_t into 5 slices (srcType, srcID, snkType, snkID, weight)
			bool srcType = extractBits(genes.at(i), 0, 0);
			bool snkType = extractBits(genes.at(i), 8, 8);
			uint8_t srcID = extractBits(genes.at(i), 1, 7);
			uint8_t snkID = extractBits(genes.at(i), 9, 15);
			int16_t weight = extractBits(genes.at(i), 16, 31); 

			Connection conn{ true };
			// divide the IDs by the number of neurons in that class. neuronClasses defined in Neurons.cpp
			if (srcType == 0) { //internal Neuron
				float num_Neurons = neuronClasses.at(2) - neuronClasses.at(1);
				float normfactor_src = num_Neurons / 128; //2^7
				conn.source = static_cast<NeuronTypes>(std::floor(srcID * normfactor_src));
			}
			else if (srcType == 1) { //sensor Neuron
				float num_Neurons = neuronClasses.at(1) - neuronClasses.at(0);
				float normfactor_src = num_Neurons / 128; //2^7
				conn.source = static_cast<NeuronTypes>(std::floor(srcID * normfactor_src));
			}
			else { throw std::out_of_range("source Index oor"); }

			if (snkType == 0) {//internal neuron
				float num_Neurons = neuronClasses.at(2) - neuronClasses.at(1);
				float normfactor_snk = num_Neurons / 128; //2^7
				conn.sink = static_cast<NeuronTypes>(std::floor(snkID * normfactor_snk));
			}
			else if (snkType == 1) {//action neuron
				float num_Neurons = neuronClasses.at(3) - neuronClasses.at(2);
				float normfactor_snk = num_Neurons / 128; //2^7
				conn.sink = static_cast<NeuronTypes>(std::floor(snkID * normfactor_snk));
			}	
			else { throw std::out_of_range("sink Index oor"); }

			conn.weight = static_cast<double>(weight * (2. * weight_factor / 65536.0f)); // range from -weight_f to +weight_f

			conn_list.at(i) = conn;
		}
		// ONLY FOR DEBUG ///////////////////////////
		for (auto adj : conn_list) {
			std::cout << static_cast<int>(adj.source) << "->" 
				<< static_cast<int>(adj.sink) << "{" << adj.weight << "}" << ", ";
		}
		std::cout << "\n";
		/////////////////////////////////////////////


		return conn_list;
	}

	uint32_t extractBits(uint32_t x, int low, int high) {
		// extract bits from position low to high (starting at 0=rightmost bit)
		uint32_t mask = ((1u << (high - low + 1)) - 1u) << low;
		return (x & mask) >> low;
	}

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_fwdAdj(std::array<Connection, numberOfGenes> conn_list) {

		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adj{}; // array of arrays; buffer is numberOfGenes --> max number of connections to a single gene.
																				   // this is done so that stack allocation is still possible

		std::array<int, NUM_NEURONS> connection_tracker{}; // temp array to track how many connections each neuron already has

		// for each connenction in conn_list, add sink, weight to fwd_adj[src]
		for (Connection conn : conn_list) {
			if (conn.valid) {
				int next_free_index = connection_tracker.at(conn.source)++;
				fwd_adj.at(conn.source).at(next_free_index) = Adjacency{ true, conn.sink, conn.weight };
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (auto adj : fwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << ", ";
			}
			std::cout << "]\n";
		}
		/////////////////////////////////////////////

		return fwd_adj;
	}

	void checkForLoops_DFS(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adj, std::array<Connection, numberOfGenes> conn_list) {// run through the fwd adjacency list and check for loops, dead ends
		// Do a DFS: 
		// create a temporary array to track which neurons were visited. 0 = unvisited, 1 = visiting, 2 = visited
		std::array<int, NUM_NEURONS> visiting_status{};
		
		bool dfs_unsuccessfull = true;
		
		while (dfs_unsuccessfull) {
			// run through the arrays in adj_list and only consider those with valid = true.
			// Only start from sensor neurons! (see neuronClasses[1])
			for (int i = 0; i < neuronClasses.at(1); i++) {

				std::array<int, numberOfGenes> path_tracker{}; // tracks the Neuron indices of the currently visited path
				size_t pathDepth = 0; //path_tracker[pathDepth++] = x --> push, x = path_tracker[--pathDepth] --> pop

				std::array<int, numberOfGenes> branch_tracker{}; // sparse set to track how many adjacencies have already been visited
				size_t currentBranchDepth{};

				path_tracker[pathDepth++] = i; // start at sensor neuron i
				std::cout << path_tracker[pathDepth - 1]; //DEBUG

				while (pathDepth > 0) {

					// for each neuron index reached, set status to 1. Take the first sink index from its adjacency and proceed.
					currentBranchDepth = branch_tracker[path_tracker[pathDepth]]++;
					if (fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).valid) { // there is a connection
						std::cout << "->" << static_cast<int>(fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).neighbour); //DEBUG

						// check if that connection closes a loop
						if (visiting_status[fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).neighbour] == 1) { // already visiting
							// Loop!
							std::cout << "Loop!";
							// delete connection fom fwd_adj: set adjacency to invalid
							fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).valid = false;
							// delete connection from conn_list: 
							delete_connection(conn_list, path_tracker[pathDepth], fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).neighbour);

							// set to visited and go back one neuron
							visiting_status.at(path_tracker[pathDepth]) = 2;
							--pathDepth; // go back one neuron
							continue;
						}

						visiting_status.at(path_tracker[pathDepth]) = 1;
						// add current neuron to path and start again
						path_tracker[pathDepth++] = fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).neighbour;
						continue;
					}
					else {// all connections visited (or no connections)
						std::cout << "->" << static_cast<int>(fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).neighbour) << "(end)"; //DEBUG
						// check if it is not an action neuron (dead end)
						if (path_tracker[pathDepth] < neuronClasses.at(2)) {
							std::cout << "dead end!"; //DEBUG
							// delete connection fom fwd_adj: set adjacency to invalid
							fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).valid = false;
							// delete connection from conn_list: 
							delete_connection(conn_list, path_tracker[pathDepth], fwd_adj.at(path_tracker[pathDepth]).at(currentBranchDepth).neighbour);
						}


						visiting_status.at(path_tracker[pathDepth]) = 2;
						--pathDepth; // go back one neuron
						continue;
					}
				}
				std::cout << "\n"; //DEBUG
			}
			std::cout << "checking for unrooted\n"; //DEBUG
			// when all sensor neurons are 2, check if there are still unvisited neurons with a non-empty adjacency list.
			for (int i = neuronClasses.at(1); i < neuronClasses.at(3); i++) {// also check action neurons to be sure 
				for (int j = 0; j < numberOfGenes; j++) {
					if (fwd_adj.at(i).at(j).valid) {// if so, these are unrooted internals linking up to internals/actions which will never fire.
						std::cout << "unrooted internal " << i << "->" << fwd_adj.at(i).at(j).neighbour << "\n"; //DEBUG
						// remove their connections in fwd_adj AND in conn_list
						fwd_adj.at(i).at(j).valid = false;
						delete_connection(conn_list, i, fwd_adj.at(i).at(j).neighbour);

						if (i > neuronClasses.at(2)) { // if its an action neuron with a connection, something went terribly wrong
							std::cerr << "Error: Action Neuron as source";
						}
					}
				}

			}
			dfs_unsuccessfull = false;
		}
	}


	void delete_connection(std::array<Connection, numberOfGenes> conn_list, int source, int sink) { // find and delete a connection with matching sources and sinks
		for (auto conn : conn_list) {
			if (conn.source == source) {
				if (conn.sink == sink) {
					conn.valid = false;
					std::cout << "deleted connection " << source << "->" << sink << "\n"; //DEBUG
					break;
				}
			}
		}
	}
}