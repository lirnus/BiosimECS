#include "Genome.h"
#include "components.h"

#include <cmath>
#include <iostream>

namespace bs {

	Entity createGenome(World* w, Entity p) {
		// create new Genome entity
		Entity newGenome = w->braintemplates_em.create();
		w->genome.add(newGenome, {}); // zero-initialize a Genome to later add components

		// generate DNA from scratch
		//std::array<uint32_t, numberOfGenes> genes = generateDNA();
		w->genome.get(newGenome).DNA = generateDNA();

		// skip the step of checkForClone because the propability of generating a clone from scratch is vanishingly low and we want maximum performance

		// map DNA2NeuronTypes, generate connection list
		std::array<Connection, numberOfGenes> conn_list = mapDNA2Connections(w->genome.get(newGenome).DNA);

		// generate fwd_adjacency list
		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adjacency = generate_fwdAdj(conn_list);

		// check for loops and dead ends, remove
		checkForLoops_DFS(fwd_adjacency, conn_list);

		// generate bwd_adjacency list
		w->genome.get(newGenome).bwd_adjacency = generate_bwdAdj(conn_list);

		// calculate topology
		calculate_topoOrder(w->genome.get(newGenome).bwd_adjacency, w->genome.get(newGenome).topoOrder);

		// new random color for that genome
		w->genome.get(newGenome).col.r = randomengine->getRandomIntCustom(0, 255);
		w->genome.get(newGenome).col.b = randomengine->getRandomIntCustom(0, 255);
		w->genome.get(newGenome).col.g = randomengine->getRandomIntCustom(0, 255);

		// return Genome entity
		return newGenome;
	}

	Entity inheritGenome(World* w, Entity p, const Genome& old_gnm) {
		std::array<uint32_t, numberOfGenes> possibly_mutated_DNA = mutateDNA(old_gnm);

		//bool silent_mut = false; // no change in connectivity or weight
		bool weight_mut = false; // the weight of one or more connections has changed
		bool neuron_mut = false; // a connection has changed

		// compare old & new DNA
		for (int i = 0; i < numberOfGenes; i++) {
			uint32_t old_gene = old_gnm.DNA.at(i);
			uint32_t new_gene = possibly_mutated_DNA.at(i);

			if (old_gene != new_gene) {
				Connection old_conn = mapDNA2Connection_single(old_gene);
				Connection new_conn = mapDNA2Connection_single(new_gene);
				// first: is the mutation in the neuronID- or the weight part?
				if (old_conn.weight != new_conn.weight) { // weight mutation
					weight_mut = true;
				}
				//second: does the old & new gene encode a different NeuronType?
				if (old_conn.source != new_conn.source || old_conn.sink != new_conn.sink) { // missense
					neuron_mut = true;
				}
				// if old_gene != new_gene, a silent mutation can always be ticked (because if the others apply, they override anyways)
				//silent_mut = true;
			}
		}

		// first check if there already is a braintemplate Entity with the same DNA. if yes, skip the initialization
		Entity clone = checkForClone(w, old_gnm.DNA);
		if (clone != INVALID) { return clone; }

		// else create a new Entity
		if (neuron_mut) {
			// new topology
			return inheritGenome_Missense(w, p, possibly_mutated_DNA, old_gnm);
		}
		else if (weight_mut) {
			// inherit genome with modifed bwd_adj
			return inheritGenome_Weight(w, p, possibly_mutated_DNA, old_gnm);
		}
		else {
			// inherit whole genome with (new) DNA
			return inheritGenome_Silent(w, p, possibly_mutated_DNA, old_gnm);
		}
	}
	Entity inheritGenome_Missense(World* w, Entity p, const std::array<uint32_t, numberOfGenes> new_dna, const Genome& old_gnm) {
		// basically createGenome without the generation of new DNA.
		// create new Genome entity
		Entity newGenome = w->braintemplates_em.create();
		w->genome.add(newGenome, {}); // zero-initialize a Genome to later add components

		w->genome.get(newGenome).DNA = new_dna;

		// generate connection list
		std::array<Connection, numberOfGenes> conn_list = mapDNA2Connections(w->genome.get(newGenome).DNA);

		// generate fwd_adjacency list
		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adjacency = generate_fwdAdj(conn_list);

		// check for loops and dead ends, remove
		checkForLoops_DFS(fwd_adjacency, conn_list);

		// generate bwd_adjacency list
		w->genome.get(newGenome).bwd_adjacency = generate_bwdAdj(conn_list);

		// calculate topology
		calculate_topoOrder(w->genome.get(newGenome).bwd_adjacency, w->genome.get(newGenome).topoOrder);

		// new random color for that genome (full variation scope)
		w->genome.get(newGenome).col = generateSimilarColor(old_gnm.col, 1.0);

		// return Genome entity
		return newGenome;
	}
	Entity inheritGenome_Weight(World* w, Entity p, std::array<uint32_t, numberOfGenes> new_dna, const Genome& old_gnm) {
		// copy new DNA, copy old topoOrder and bwd_adj but update the weight for all bwd_adj connections.
		// create new Genome entity
		Entity newGenome = w->braintemplates_em.create();
		w->genome.add(newGenome, {}); // zero-initialize a Genome to later add components

		w->genome.get(newGenome).DNA = new_dna;

		w->genome.get(newGenome).topoOrder = old_gnm.topoOrder;

		w->genome.get(newGenome).col = generateSimilarColor(old_gnm.col, 0.5);

		std::array<Connection, numberOfGenes> newConns = mapDNA2Connections(new_dna);

		w->genome.get(newGenome).bwd_adjacency = old_gnm.bwd_adjacency;

		// update weights
		for (Connection& conn : newConns) {
			for (int i = 0; i < numberOfGenes; i++) {
				if (w->genome.get(newGenome).bwd_adjacency.at(conn.sink).at(i).neighbour == conn.source) {
					w->genome.get(newGenome).bwd_adjacency.at(conn.sink).at(i).weight = conn.weight;
				}
			}
		}

		return newGenome;
	}
	Entity inheritGenome_Silent(World* w, Entity p, const std::array<uint32_t, numberOfGenes> new_dna, const Genome& old_gnm) {
		// copy new DNA, copy old bwd_adj and topoOrder.
		// create new Genome entity
		Entity newGenome = w->braintemplates_em.create();
		w->genome.add(newGenome, {}); // zero-initialize a Genome to later add components

		w->genome.get(newGenome).DNA = new_dna;

		w->genome.get(newGenome).topoOrder = old_gnm.topoOrder;

		w->genome.get(newGenome).col = generateSimilarColor(old_gnm.col, 0.5);

		w->genome.get(newGenome).bwd_adjacency = old_gnm.bwd_adjacency;

		return newGenome;
	}

	std::array<uint32_t, numberOfGenes> mutateDNA(const Genome& gnome) {
		std::array<uint32_t, numberOfGenes> possiblyMutatedDNA;

		for (int n = 0; n > numberOfGenes; n++) {
			uint32_t dna_copy = gnome.DNA.at(n);
			for (int i = 0; i < 32; i++) {
				if (randomengine->getRandom01() < mutationRate) { dna_copy ^= (1u << i); } // bit-masking and NOT-operator
			}
			possiblyMutatedDNA.at(n) = dna_copy;
		}
		return possiblyMutatedDNA;
	}

	Color generateSimilarColor(const Color old_c, float factor) {
		Color newColor{};

		newColor.r = std::clamp(static_cast<int>(old_c.r) + randomengine->getRandomCustom(-color_variation * factor, color_variation * factor), 0, 255);
		newColor.g = std::clamp(static_cast<int>(old_c.g) + randomengine->getRandomCustom(-color_variation * factor, color_variation * factor), 0, 255);
		newColor.b = std::clamp(static_cast<int>(old_c.b) + randomengine->getRandomCustom(-color_variation * factor, color_variation * factor), 0, 255);

		return newColor;
	}


	std::array<uint32_t,numberOfGenes> generateDNA() {
		std::array<uint32_t, numberOfGenes> genes_list{};

		for (int i = 0; i < numberOfGenes; i++) {
			genes_list.at(i)= randomengine->getRandom32b();
		}

		return genes_list;
	}
	
	Entity checkForClone(World* w, std::array<uint32_t, numberOfGenes> dna) {
		// iterate through the genomes-components and look for potential matches
		Entity match = INVALID;
		for (const Entity& e : w->genome.get_entities()) {
			if (w->genome.get(e).DNA == dna) {
				match = e;
				return match;
			}
		}
		return match;
	}

	std::array<Connection, numberOfGenes> mapDNA2Connections(std::array<uint32_t, numberOfGenes>&  genes) {

		std::array<Connection, numberOfGenes> conn_list{};

		for (int i = 0; i < numberOfGenes; i++) {
			conn_list.at(i) = mapDNA2Connection_single(genes.at(i));
		}

		// ONLY FOR DEBUG ///////////////////////////
		/*for (auto adj : conn_list) {
			std::cout << static_cast<int>(adj.source) << "->" 
				<< static_cast<int>(adj.sink) << "{" << adj.weight << "}." << adj.valid << ", ";
		}
		std::cout << "\n";*/
		/////////////////////////////////////////////

		return conn_list;
	}

	Connection mapDNA2Connection_single(const uint32_t& gene) {

		bool srcType = extractBits(gene, 0, 0);
		bool snkType = extractBits(gene, 8, 8);
		uint8_t srcID = extractBits(gene, 1, 7);
		uint8_t snkID = extractBits(gene, 9, 15);
		int16_t weight = extractBits(gene, 16, 31);

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

		return conn;
	}

	uint32_t extractBits(uint32_t x, int low, int high) {
		// extract bits from position low to high (starting at 0=rightmost bit)
		uint32_t mask = ((1u << (high - low + 1)) - 1u) << low;
		return (x & mask) >> low;
	}

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_fwdAdj(const std::array<Connection, numberOfGenes>& conn_list) {

		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> fwd_adj{}; // array of arrays; buffer is numberOfGenes --> max number of connections to a single gene.
																				   // this is done so that stack allocation is still possible

		std::array<int, NUM_NEURONS> connection_tracker{}; // temp array to track how many connections each neuron already has

		// for each connenction in conn_list, add sink, weight to fwd_adj[src]
		for (const Connection& conn : conn_list) {
			if (conn.valid) {

				// check if that connection already exists. if yes, just add to the weight factor
				bool multiconnect = false;
				for (Adjacency& adj : fwd_adj.at(conn.source)) {
					if (conn.sink == adj.neighbour) {
						adj.weight += conn.weight;
						multiconnect = true;
					}
				}

				if (!multiconnect) {
					int next_free_index = connection_tracker.at(conn.source)++;
					fwd_adj.at(conn.source).at(next_free_index) = Adjacency{ true, conn.sink, conn.weight };
				}
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		/*for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : fwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}*/
		/////////////////////////////////////////////

		return fwd_adj;
	}

	void checkForLoops_DFS(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& fwd_adj, 
		std::array<Connection, numberOfGenes>& conn_list) {
		
		// create a temporary array to track which neurons were visited. 0 = unvisited, 1 = visiting, 2 = visited
		std::array<int, NUM_NEURONS> visiting_status{};

		//bool dfs_unsuccessfull = true;

		// small lambda to check if a neuron has any valid connections:
		auto isvalid = [fwd_adj](int idx) {
			int sum = 0;
			for (Adjacency adj : fwd_adj.at(idx)) {
				sum += adj.valid;
			}
			return sum > 0;
		};

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
restart:
		for (int root = 0; root < neuronClasses.at(1); root++) {

			//std::cout << "root: " << root << "\n";
			if (visiting_status[root] == 2) continue; //this neuron is already fully visited

			std::array<int, numberOfGenes> path_tracker{}; // tracks the Neuron indices of the currently visited path
			size_t pathDepth = 0; //path_tracker[pathDepth++] = x --> push, x = path_tracker[--pathDepth] --> pop

			std::array<int, NUM_NEURONS> branch_tracker{}; // sparse set to track how many adjacencies have already been visited
			size_t currentBranchDepth{};


			path_tracker[pathDepth++] = root; // start at sensor neuron i
			visiting_status[root] = 1;

			while (pathDepth > 0) {
				size_t top = pathDepth - 1;
				int currentNode = path_tracker[top]; 
					
				// get next branch index for currentNode
				int& branchIdx = branch_tracker[currentNode];

				if (static_cast<size_t>(branchIdx) >= numberOfGenes) { // a neuron is only finished if all branch indices have been checked

					// no more adjacencies -> finish this node
					visiting_status[currentNode] = 2;
					--pathDepth; // pop
					continue;
				}

				// get the neighbour
				Adjacency& adj = fwd_adj.at(currentNode).at(branchIdx);
				branchIdx++;

				// check before if the connection to the neighbour is valid
				if (!adj.valid) { // if false, go to next branch index
					continue;
				}

				// skip selfInput Connections - they don't count as loops but also don't lead to another neuron
				if (currentNode == adj.neighbour) { 
					//std::cout << "selfInput\n";
					continue; 
				}

				// check visiting status of neighbour
				if (visiting_status[adj.neighbour] == 1) { // cycle closed
					//std::cout << "LOOP detected: " << currentNode << "->" << static_cast<int>(adj.neighbour) << "\n";

					// invalidate/delete connection
					delete_connection(fwd_adj, conn_list, currentNode, adj.neighbour);

					visiting_status.fill(0);
					
					// RESTART the whole algorithm
					goto restart;
				}
				else if (visiting_status[adj.neighbour] == 0) { // currently unvisited neighbour
					visiting_status[adj.neighbour] = 1;

					if (pathDepth < path_tracker.size()) {
						path_tracker[pathDepth++] = adj.neighbour;

						// check one step ahead if the neighbour has any connections itself and if its NOT an action neuron
						if (isvalid(adj.neighbour) &&
							adj.neighbour < neuronClasses.at(2)) {// dead end

							//std::cout << "DEAD END: " << currentNode << "->" << static_cast<int>(adj.neighbour) << "\n";

							// invalidate/delete connection
							delete_connection(fwd_adj, conn_list, currentNode, adj.neighbour);
						}

					}
					else { std::cerr << "Error: pathDepth overflow\n"; break; }

					continue;
				}
				//else visiting status 2 (already visited). Nothing to do
				continue;

			} // end while pathDepth > 0

		} // end for roots

		//std::cout << "checking for unrooted\n"; //DEBUG
		// when all sensor neurons are 2, check if there are still unvisited neurons with a non-empty adjacency list.
		for (int i = neuronClasses.at(1); i < neuronClasses.at(3); i++) {// also check action neurons to be sure 
			for (int j = 0; j < numberOfGenes; j++) {
				if (visiting_status.at(i) == 0 && fwd_adj.at(i).at(j).valid) {// if so, these are unrooted internals linking up to internals/actions which will never fire.

					// remove their connections in fwd_adj AND in conn_list
					delete_connection(fwd_adj, conn_list, i, fwd_adj.at(i).at(j).neighbour);

					if (i > neuronClasses.at(2)) { // if its an action neuron with a connection, something went terribly wrong
						std::cerr << "Error: Action Neuron as source";
					}
				}
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		/*std::cout << "visiting status: {";
		for (auto elem : visiting_status) { std::cout << elem << ","; }
		std::cout << "}\n";*/

		/*for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : fwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}*/

		/*for (auto adj : conn_list) {
			std::cout << static_cast<int>(adj.source) << "->"
				<< static_cast<int>(adj.sink) << "{" << adj.weight << "}." << adj.valid << ", ";
		}
		std::cout << "\n";*/
		/////////////////////////////////////////////

	}

	std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> generate_bwdAdj(const std::array<Connection, numberOfGenes>& conn_list) {
		std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> bwd_adj{}; // array of arrays; buffer is numberOfGenes --> max number of connections to a single gene.
		// this is done so that stack allocation is still possible

		std::array<int, NUM_NEURONS> connection_tracker{}; // temp array to track how many connections each neuron already has

		// for each connenction in conn_list, add source, weight to bwd_adj[snk]
		for (const Connection& conn : conn_list) {
			if (conn.valid) {
				// check if that connection already exists. if yes, just add to the weight factor
				bool multiconnect = false;
				for (Adjacency& adj : bwd_adj.at(conn.sink)) {
					if (adj.valid && conn.source == adj.neighbour) {
						adj.weight += conn.weight;
						multiconnect = true;
						//std::cout << "MULTICONNECT PREVENTED: " << static_cast<int>(conn.source) << static_cast<int>(conn.sink) <<"\n";
					}
				}

				if (!multiconnect) {
					int next_free_index = connection_tracker.at(conn.sink)++;
					bwd_adj.at(conn.sink).at(next_free_index) = Adjacency{ true, conn.source, conn.weight };
				}
			}
		}

		// ONLY FOR DEBUG ///////////////////////////
		/*std::cout << "backwards:\n";
		for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : bwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}*/
		/////////////////////////////////////////////

		return bwd_adj;
	}

	void calculate_topoOrder(const std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS>& bwd_adj, std::vector<NeuronTypes>& topoOrder) {

		/*
		* Do a BFS (Breadth-First search) through the backwards adjacency list.
		* To do this, step through bwd_adj, beginning at the action neurons. add every adjacency neighbour to a queue
		* (that is in reality an array, to keep things on stack: numberOfGenes * 3 of buffer (max *2 should be necessary)). 
		Go through the whole graph like this, and when encountering
		* a neuron that already was visited, invalidate all previous occurrences, so that only one valid neuron of every type
		* exists.
		* In the last step, check if there really only are at max NUM_NEURONS valid neurons in the array. Then iterate through
		* The array BACKWARDS and if a neruon is valid, push it to the topoOrder vector.
		*/

		std::array<NeuronTypes, numberOfGenes * 3> bfs_queue{};
		std::array<bool, numberOfGenes * 3> track_validity{};

		int next_free_index = 0;
		int current_step = 0;

		// start at every action neuron. Each neighbour in the adj list is added to a queue (array).
		for (int root = neuronClasses.at(2); root < neuronClasses.at(3); root++) {

			if (bwd_adj.at(root).at(0).valid) {
				bfs_queue.at(next_free_index) = static_cast<NeuronTypes>(root);
				//std::cout << "root: " << static_cast<int>(bfs_queue.at(next_free_index)) << "\n";
				track_validity.at(next_free_index) = true;
				next_free_index++;

				// The queue is continuously cleared. If a node with neighbours is found, its neighbours also get added to the queue.
				while (true) {

					// DEBUG /////
					//std::cout << "current queue: {";
					//for (int m = 0; m < next_free_index; m++) { std::cout << static_cast<int>(bfs_queue[m]) << ", "; }
					//std::cout << "}\n";

					// check if currentStep has catched up with nextFreeIndex. if yes, jump to next branch
					if (current_step == next_free_index) {
						//std::cout << "end of queue, jump to next root\n";
						break;
					}

					// else, add all neighbours to queue
					for (const Adjacency& adj : bwd_adj.at(bfs_queue[current_step])) {
						if (adj.valid) {
							
							//if selfInput, skip this neuron (don't add it to the queue)
							if (bfs_queue[current_step] == adj.neighbour) {
								//std::cout << "SELFINPUT"<< static_cast<int>(bfs_queue[current_step]) << static_cast<int>(adj.neighbour) << "\n";
								continue;
							}

							//std::cout << "neighbour: " << static_cast<int>(adj.neighbour) << "\n";
							//check if the next neuron already was visited
							for (size_t i = 0; i < next_free_index; i++) {
								if (bfs_queue[i] == adj.neighbour) {
									//if yes, "delete" it by setting its validity to false
									track_validity[i] = false;
									//std::cout << "invalidated at position " << i << "\n";
								}
							}

							//add to queue (if not a selfInput)
							//std::cout << static_cast<int>(adj.neighbour) << " at " << next_free_index << "\n";
							bfs_queue.at(next_free_index) = adj.neighbour;
							track_validity.at(next_free_index) = true;
							next_free_index++;
						}
					}
					//std::cout << "current step: " << current_step << "\n";
					current_step++;
				} //end of while true
			}
		} //end of for root
		
		//quick check
		int sum = [track_validity]() { 
			int sum_up = 0;
			for (const bool i : track_validity) { if (i) sum_up++; }
			return sum_up;
			}();
		assert(sum <= NUM_NEURONS);

		//now step through the array in reverse order and push to topoOrder
		for (int i = numberOfGenes * 3 - 1; i >= 0; i--) {
			if (track_validity.at(i)) { // if still valid

				topoOrder.push_back(bfs_queue.at(i));
			}
		}

		// DEBUG //////////////
		/*std::cout << "bfs array: {";
		for (auto elem : bfs_queue) {
			std::cout << static_cast<int>(elem) << ", ";
		}
		std::cout << "}\n";*/
		/*std::cout << "topoOrder of Neurons: {";
		for (auto elem : topoOrder) {
			std::cout << static_cast<int>(elem) << ", ";
		}
		std::cout << "}\n";*/
		////////////////////////
	}


	void delete_connection(std::array<std::array<Adjacency, numberOfGenes>, NUM_NEURONS> &fwd_adj, std::array<Connection, numberOfGenes> &conn_list, int source, int sink) { // find and delete any connections with matching sources and sinks
		for (Connection& conn : conn_list) {
			if (conn.source == source) {
				if (conn.sink == sink) {
					conn.valid = false;
					//std::cout << "deleted connection " << static_cast<int>(source) << "->" << static_cast<int>(sink) << "\n"; //DEBUG
				}
			}
		}

		for (Adjacency& snk : fwd_adj.at(source)) {
			if (snk.neighbour == sink) {
				snk.valid = false;
				//std::cout << "set [" << static_cast<int>(source) << "]->" 
				//	<< static_cast<int>(snk.neighbour) << "." << static_cast<int>(snk.valid) << " invalid" << "\n"; //DEBUG
			}
		}
	}
}