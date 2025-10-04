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

	std::array<Connection, numberOfGenes> mapDNA2Connections(std::array<uint32_t, numberOfGenes>&  genes) {

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
				conn.source = static_cast<NeuronTypes>(std::floor(srcID * normfactor_src) + neuronClasses.at(1));
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
				conn.sink = static_cast<NeuronTypes>(std::floor(snkID * normfactor_snk) + neuronClasses.at(1));
			}
			else if (snkType == 1) {//action neuron
				float num_Neurons = neuronClasses.at(3) - neuronClasses.at(2);
				float normfactor_snk = num_Neurons / 128; //2^7
				conn.sink = static_cast<NeuronTypes>(std::floor(snkID * normfactor_snk) + neuronClasses.at(2));
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

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_fwdAdj(std::array<Connection, numberOfGenes>& conn_list) {

		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adj{}; // array of arrays; buffer is numberOfGenes --> max number of connections to a single gene.
																				   // this is done so that stack allocation is still possible

		std::array<int, NUM_NEURONS> connection_tracker{}; // temp array to track how many connections each neuron already has

		// for each connenction in conn_list, add sink, weight to fwd_adj[src]
		for (Connection& conn : conn_list) {
			if (conn.valid) {
				int next_free_index = connection_tracker.at(conn.source)++;
				fwd_adj.at(conn.source).at(next_free_index) = Adjacency{ true, conn.sink, conn.weight };
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : fwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}
		/////////////////////////////////////////////

		return fwd_adj;
	}

	void checkForLoops_DFS(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& fwd_adj, 
		std::array<Connection, numberOfGenes>& conn_list) {
		
		// create a temporary array to track which neurons were visited. 0 = unvisited, 1 = visiting, 2 = visited
		std::array<int, NUM_NEURONS> visiting_status{};

		//bool dfs_unsuccessfull = true;


		/*
		DFS = Depth-First Search. Goes through the graph of connections by following a single path until its end, before backtracking
		and finding the next path.
		This function only uses Arrays, runs purely on stack, and so is very fast. 
		It catches almost all loops and dead ends, but not all, especially when multiple internal neurons are linked with each other.
		But these errors are minor and will in some cases be flushed out when converted to bwd_adj and topoOrder.

		For this case the deletion of "dead ends" that don't lead to action neurons is superfluous, because they will never reach the 
		topoOrder stage. They could take up extra space in the bwd_adj list but if it has a static size this wouldn't be a problem.
		It could shave off a few milliseconds each time a genome gets created, so I could test if it makes a big difference
		*/
		
		// run through the arrays in adj_list and only consider those with valid = true.
		// Only start from sensor neurons! (see neuronClasses[1])
		for (int root = 0; root < neuronClasses.at(1); root++) {

			if (visiting_status[root] == 2) continue; //this neuron is already fully visited

			std::array<int, numberOfGenes> path_tracker{}; // tracks the Neuron indices of the currently visited path
			size_t pathDepth = 0; //path_tracker[pathDepth++] = x --> push, x = path_tracker[--pathDepth] --> pop

			std::array<int, NUM_NEURONS> branch_tracker{}; // sparse set to track how many adjacencies have already been visited
			size_t currentBranchDepth{};

			path_tracker[pathDepth++] = root; // start at sensor neuron i
			std::cout << "start root " << root << "\n"; //DEBUG

			while (pathDepth > 0) {
				size_t top = pathDepth - 1;
				int currentNode = path_tracker[top]; 
					
				// DEBUG print
				std::cout << "path (depth=" << pathDepth << "): ";
				for (size_t i = 0; i < pathDepth; ++i) std::cout << path_tracker[i] << (i + 1 < pathDepth ? "->" : "");
				std::cout << "\n";

				// get next branch index for currentNode
				int& branchIdx = branch_tracker[currentNode];

				if (static_cast<size_t>(branchIdx) >= numberOfGenes) {
					// no more adjacencies -> finish this node
					std::cout << "end of adjacency list reached\n";
					visiting_status[currentNode] = 2;
					--pathDepth; // pop
					continue;
				}

				// get the neighbour
				Adjacency& adj = fwd_adj.at(currentNode).at(branchIdx);
				branchIdx++;

				// check before if the connection to the neighbour is valid
				if (!adj.valid) { // if false, current node has no more connections
					
					visiting_status.at(currentNode) = 2;
					--pathDepth;
					continue;
				}

				// skip selfInput Connections - they don't count as loops but also don't lead to another neuron
				if (currentNode == adj.neighbour) { 
					std::cout << "selfInput\n";
					continue; 
				}

				// check visiting status of neighbour
				if (visiting_status[adj.neighbour] == 1) { // cycle closed
					std::cout << "LOOP detected: " << currentNode << "->" << static_cast<int>(adj.neighbour) << "\n";

					// invalidate/delete connection
					//adj.valid = false;
					delete_connection(fwd_adj, conn_list, currentNode, adj.neighbour);

					visiting_status.at(currentNode) = 2;
					//--pathDepth; // go back one neuron
					for (int node : path_tracker) { visiting_status[node] = 0; } // set all visiting neurons to 0
					root--; // do the same iteration again
					continue;
				}
				else if (visiting_status[adj.neighbour] == 0) { // currently unvisited neighbour
					visiting_status[adj.neighbour] = 1;

					if (pathDepth < path_tracker.size()) {
						path_tracker[pathDepth++] = adj.neighbour;

						// check one step ahead if the neighbour has any connections itself and if its NOT an action neuron
						if (!fwd_adj.at(adj.neighbour).at(0).valid &&
							adj.neighbour < neuronClasses.at(2)) {// dead end

							std::cout << "DEAD END: " << currentNode << "->" << static_cast<int>(adj.neighbour) << "\n";

							// invalidate/delete connection
							delete_connection(fwd_adj, conn_list, currentNode, adj.neighbour);
						}

					}
					else { std::cerr << "Error: pathDepth overflow\n"; break; }

					continue;
				}
				//else visiting status 2 (already visited). Nothing to do
				std::cout << "already visited " << static_cast<int>(adj.neighbour) << "\n";
				continue;
			} // end while pathDepth > 0
		} // end for roots

		//DEBUG
		std::cout << "visiting status: {";
		for (auto elem : visiting_status) { std::cout << elem << ","; }
		std::cout << "}\n";

		std::cout << "checking for unrooted\n"; //DEBUG
		// when all sensor neurons are 2, check if there are still unvisited neurons with a non-empty adjacency list.
		for (int i = neuronClasses.at(1); i < neuronClasses.at(3); i++) {// also check action neurons to be sure 
			for (int j = 0; j < numberOfGenes; j++) {
				if (visiting_status.at(i) == 0 && fwd_adj.at(i).at(j).valid) {// if so, these are unrooted internals linking up to internals/actions which will never fire.
					std::cout << "unrooted internal " << i << "->" << static_cast<int>(fwd_adj.at(i).at(j).neighbour) << "\n"; //DEBUG
					// remove their connections in fwd_adj AND in conn_list
					//fwd_adj.at(i).at(j).valid = false;
					delete_connection(fwd_adj, conn_list, i, fwd_adj.at(i).at(j).neighbour);

					if (i > neuronClasses.at(2)) { // if its an action neuron with a connection, something went terribly wrong
						std::cerr << "Error: Action Neuron as source";
					}
				}
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : fwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}
		/////////////////////////////////////////////

	}

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_bwdAdj(std::array<Connection, numberOfGenes>& conn_list) {
		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> bwd_adj{}; // array of arrays; buffer is numberOfGenes --> max number of connections to a single gene.
		// this is done so that stack allocation is still possible

		std::array<int, NUM_NEURONS> connection_tracker{}; // temp array to track how many connections each neuron already has

		// for each connenction in conn_list, add source, weight to fwd_adj[snk]
		for (Connection& conn : conn_list) {
			if (conn.valid) {
				int next_free_index = connection_tracker.at(conn.sink)++;
				bwd_adj.at(conn.sink).at(next_free_index) = Adjacency{ true, conn.source, conn.weight };
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : bwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}
		/////////////////////////////////////////////

		return bwd_adj;
	}


	void delete_connection(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> &fwd_adj, std::array<Connection, numberOfGenes> &conn_list, int source, int sink) { // find and delete any connections with matching sources and sinks
		for (Connection& conn : conn_list) {
			if (conn.source == source) {
				if (conn.sink == sink) {
					conn.valid = false;
					std::cout << "deleted connection " << static_cast<int>(source) << "->" << static_cast<int>(sink) << "\n"; //DEBUG
					break;
				}
			}
		}

		for (Adjacency& snk : fwd_adj.at(source)) {
			if (snk.neighbour == sink) {
				snk.valid = false;
				std::cout << "set [" << static_cast<int>(source) << "]->" 
					<< static_cast<int>(snk.neighbour) << "." << static_cast<int>(snk.valid) << " invalid" << "\n"; //DEBUG
			}
		}
	}
}