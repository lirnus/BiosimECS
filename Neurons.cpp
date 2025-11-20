#include "Neurons.h"
#include "World.h"
//#include "components.h"
#include <cassert>
#include <iostream>


namespace bs {

	/*const std::vector<int> neuronClasses = { 0, 4, 7, 11 };*/


	//using NeuronFunc = void(*)(World* w, Entity e);
	std::vector<NeuronFunc> funcTable;
	//std::array < NeuronFunc, NUM_NEURONS > funcTable{};

	/*void initFuncTable() {
		funcTable.fill(nullptr);

		funcTable[sX_POS] = &xPos_neuronfunc;
		funcTable[sY_POS] = &yPos_neuronfunc;
		funcTable[sPOP_DENSITY_FWD] = &popDensityFwd_neuronfunc;
		funcTable[sAGE] = age_neuronfunc;
		funcTable[iINT_1] = nullptr;
		funcTable[iINT_2] = nullptr;
		funcTable[iINT_3] = nullptr;
		funcTable[aMOVE_W] = moveW_neuronfunc;
		funcTable[aMOVE_E] = moveE_neuronfunc;
		funcTable[aMOVE_N] = moveN_neuronfunc;
		funcTable[aMOVE_S] = moveS_neuronfunc;
	}*/

	void initNeuronFuncTable() {// fill up funcTable and neuronClasses
		
		int index_counter{ 0 };
		bool allN = activeNeurons->allNeurons;

		//s
		neuronClasses.push_back(index_counter); // { 0 }
		if (allN || activeNeurons->xPos)			funcTable.push_back(&xPos_neuronfunc); index_counter++;
		if (allN || activeNeurons->yPos)			funcTable.push_back(&yPos_neuronfunc); index_counter++;
		if (allN || activeNeurons->popDensityFwd)	funcTable.push_back(&popDensityFwd_neuronfunc); index_counter++;
		if (allN || activeNeurons->age)				funcTable.push_back(&age_neuronfunc); index_counter++;
		neuronClasses.push_back(index_counter); // { 0, numSensors }
		//i
		for (int i = 0; i < activeNeurons->numInternals; i++) {
			funcTable.push_back(nullptr);
			index_counter++;
		}
		neuronClasses.push_back(index_counter); // { 0, numSensors, numInternals }
		//a
		if (allN || activeNeurons->moveW)			funcTable.push_back(&moveW_neuronfunc); index_counter++;
		if (allN || activeNeurons->moveE)			funcTable.push_back(&moveE_neuronfunc); index_counter++;
		if (allN || activeNeurons->moveN)			funcTable.push_back(&moveN_neuronfunc); index_counter++;
		if (allN || activeNeurons->moveS)			funcTable.push_back(&moveS_neuronfunc); index_counter++;
		if (allN || activeNeurons->emitPher)		funcTable.push_back(&emitPheromone_neuronfunc); index_counter++;
		neuronClasses.push_back(index_counter); // { 0, numSensors, numInternals, NUM_NEURONS }
	}

	// AXON FUNCTIONS ////////////////////////////////////////////////////////////////////////////////

	void update_BrainState(
		const int& neuron, 
		World* w, 
		Entity pixie
	) {
		BrainState& bs = w->brainstate.get(pixie);
		bs.lastStepOutputs = bs.outputs;
		bs.outputs[neuron] = funcTable[neuron](w, pixie);
	}

	void sumAndClamp(
		const std::array<std::array<Adjacency, MAX_ADJ>, MAX_NEURONS>& bwd_adj,
		const int& neuron, 
		World* w, 
		Entity p
	) {
		// debug
		//std::cout << static_cast<int>(neuron) << "\n";

		float input_sum = 0.0;
		for (Adjacency adj : bwd_adj.at(neuron)) {
			if (adj.valid) { // add up all the outputs from neighbouring sources
				// if selfInput, get value from last round
				BrainState& bs = w->brainstate.get(p);
				if (adj.neighbour == neuron) {
					input_sum += bs.lastStepOutputs[adj.neighbour];
				}
				else {
					// maybe consider a check if the value actually is set (if topology is correct, this should be the case)
					if (bs.outputs[adj.neighbour] == VOID) { throw std::invalid_argument("VOID passed as input"); } // This can be cut for performance enhancement
					input_sum += bs.outputs.at(adj.neighbour);
				}
				
			}
		}

		w->brainstate.get(p).outputs.at(neuron) = std::tanh(input_sum);
	}

	void evaluate(
		const std::array<std::array<Adjacency, MAX_ADJ>, MAX_NEURONS>& bwd_adj,
		const int& neuron,
		World* w,
		Entity p
	) {
		sumAndClamp(bwd_adj, neuron, w, p);

		if (randomengine->getRandom01() < w->brainstate.get(p).outputs.at(neuron)) {
			// fire neuron
			funcTable[neuron](w, p);
		}
	}


	void execute_staticBrain(World* w, Entity p) {
		// execute topoOrder functions
		Genome& pixie_gnm = w->genome.get(w->PixieGenomes.get(p));
		std::vector<uint8_t>& topoOrder = pixie_gnm.topoOrder;
		std::array<std::array<Adjacency, MAX_ADJ>, MAX_NEURONS>& bwd_adj = pixie_gnm.bwd_adjacency;
		
		// DEBUG: ////////////////
		/*std::cout << "topoOrder: {";
		for (auto& i : topoOrder) { std::cout << static_cast<int>(i) << ", "; }
		std::cout << "}\nbwd_adj:\n";
		for (int idx = 0; idx < NUM_NEURONS; idx++) {
			std::cout << "[" << idx << "] [";
			for (Adjacency adj : bwd_adj.at(idx)) {
				std::cout << static_cast<int>(adj.neighbour) << "/";
				std::cout << static_cast<int>(adj.valid) << ", ";
			}
			std::cout << "]\n";
		}*/
		//////////////////////////

		for (auto& neuron : topoOrder) {
			// for each, check which neuronClass the index belongs to
			
			// if sensor: directly call sensor Function via update_BrainState()
			if (neuron >= neuronClasses.at(0) && neuron < neuronClasses.at(1)) {
				update_BrainState(neuron, w, p);
			}
			
			// if internal: call sumAndClamp(bwd_adj, neuron), which also automatically updates BrainState
			if (neuron >= neuronClasses.at(1) && neuron < neuronClasses.at(2)) {
				sumAndClamp(bwd_adj, neuron, w, p);
			}
			// if action: call Evaluate(), which uses sumAndClamp to update BrainState, and then evaluates if the action should be fired.
			if (neuron >= neuronClasses.at(2) && neuron < neuronClasses.at(3)) {
				evaluate(bwd_adj, neuron, w, p);
			}

		}
	}


	// NEURON FUNCTIONS //////////////////////////////////////////////////////////////////////////////

	// SENSOR //////////////////////////////////////

	float xPos_neuronfunc(World* w, Entity p) {
		// get the x position of the pixie
		assert(w->Pos.has(p));
		int x_pos = w->Pos.get(p).xPos;

		// divide it by the total x-size of the grid
		return x_pos / static_cast<float>(worldParams->gridSizeX);

	}

	float yPos_neuronfunc(World* w, Entity p) {
		// get the x position of the pixie
		assert(w->Pos.has(p));
		int y_pos = w->Pos.get(p).yPos;

		// divide it by the total x-size of the grid
		return y_pos / static_cast<float>(worldParams->gridSizeX);
	}

	float popDensityFwd_neuronfunc(World* w, Entity p) {

		// I want to keep this function stack-compatible! No heap allocation if possible

		// get the view axis, compute the denstity 'quantum' for each pixie and sum it up
		// divide the sum by twice the search - Radius to get a value between 0 and 1

		float cache{};

		float viewAxis = w->facing.get(p);
		
		const Neighbourhood& nbhd = scanNeighbourhood(w, p);

		for (int i = 0; i < nbhd.neighbours.size(); i++) {
			if (Entity nb = nbhd.neighbours.at(i); 
				nb != BARRIER && nb != FOOD) {

				float factor = std::cos(nbhd.relAngles.at(i) - viewAxis);
				cache += (1 / nbhd.distances.at(i) * factor);
			}
		}

		// heuristic norming factor to get similar results between 0 and 1 for all search radii
		float norming_factor = 1.9 * (w->searchRadius.get(p) - 0.5);

		float out = (cache / norming_factor) / 2 + 0.5; // centered around 0.5

		//std::cout << "population density pixie " << p << ": " << out << "\n";
		return std::clamp(out, static_cast<float>(0), static_cast<float>(1));
	}

	float age_neuronfunc(World* w, Entity p) {
		return static_cast<float>(generationAge) / worldParams->numberOfSimSteps;
	}

	float onOff_neuronfunc(World* w, Entity p) {
		return 1.0;//w->brainstate.get(p).lastStepOutputs
	}

	// ACTION //////////////////////////////////////

	float moveW_neuronfunc(World* w, Entity p) {

		w->move_urge.get(p).moveX -= 1;
		w->queueForMove.insert(p);

		return VOID;
	}

	float moveE_neuronfunc(World* w, Entity p) {

		w->move_urge.get(p).moveX += 1;
		w->queueForMove.insert(p);

		return VOID;
	}

	float moveN_neuronfunc(World* w, Entity p) {

		w->move_urge.get(p).moveY += 1;
		w->queueForMove.insert(p);

		return VOID;
	}

	float moveS_neuronfunc(World* w, Entity p) {

		w->move_urge.get(p).moveY -= 1;
		w->queueForMove.insert(p);

		return VOID;
	}

	float emitPheromone_neuronfunc(World* w, Entity p) {

		// increase the pheromone levels around the pixie, decaying quadratically with distance
		int searchR = static_cast<int>(std::ceil(w->searchRadius.get(p)));
		Position pos = w->Pos.get(p);

		for (int r = pos.yPos - searchR; r < pos.yPos + searchR; r++) {
			for (int c = pos.xPos - searchR; c < pos.xPos + searchR; c++) {

				if (w->isInBounds(r, c)) {
					int dy = r - pos.yPos;
					int dx = c - pos.xPos;
					if (isInSearchRadius(dx, dy, w, p)) {

						int dst = std::sqrt(dy * dy + dx * dx)+1;

						//float value_f = 255.0f / (2.0f * quad_dst); quadratic decay (very fast)
						float value_f = 128.0f * std::exp((-2.0f * dst) / (searchR * searchR)); // gaussian-similar decay curve
						uint8_t value = static_cast<uint8_t>(value_f);
						w->setPheromoneCell(r, c, value);
					}
				}
			}
		}

		return VOID;
	}

	//future note: OnOff-neuron doesn't need a genome-wide variable "isOn", it can just return the last output again and again.
	//if the state should be switched, setOnOff can directly change lastOutputs to switch the variable
}