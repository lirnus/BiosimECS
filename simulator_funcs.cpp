#include "simulator_funcs.h"
#include "Genome.h" //incl Neurons, World, ecs_framework -> global_params

#include <algorithm>
#include <memory>
#include <iostream>
#include <array>

#include <omp.h> // Multithread

namespace bs {
	void spawnPixie(World* w) {
		// create a new Pixie entity and place it on the grid
		Entity newPixie = w->pixie_em.create();								// this part until the genome creation
																			// is shared with inheritPixie --> should I summarize as 1 function?
		int randX;
		int randY;
		while (true) {
			randX = randomengine->getRandomIntCustom(0, gridsizeX - 1);
			randY = randomengine->getRandomIntCustom(0, gridsizeY - 1);
			if (w->getGridCell(randY, randX) == EMPTY) { break; }
		}
		w->Pos.add(newPixie, Position{ randY, randX });
		w->setGridCell(w->Pos.get(newPixie), newPixie);

		w->facing.add(newPixie, 0.0);

		w->move_urge.add(newPixie, MoveUrge{ 0, 0 });

		w->brainstate.add(newPixie, BrainState{}); // empty brain 

		w->fitness.add(newPixie, 1.0);

		w->searchRadius.add(newPixie, defaultSearchRadius);

		w->pixie_neighbourhood.add(newPixie, Neighbourhood{});

		// create a genome:
		w->PixieGenomes.add(newPixie, createGenome(w, newPixie));
			
	}
	/*when inheriting a pixie, choose a pixie from the old population (with respect to their fitness) and copy their genome.
	* FUNCTION: select() 
	* this gets done in the generation before! The genome is saved in a temporary array.
	* 
	* While copying some mutations can happen. Then there are four cases:
	* 1. no mutations. The whole genome (DNA, bwd_adj, topoOrder) can be copied directly into the new pixie.
	* 2. only weight mutations. The modified genome (DNA, bwd_adj, topoOrder) can be copied, but the weight factor for that gene in 
	*	 bwd_adj has to be adjusted. The color of that pixie should vary only slightly from the mother strand.
	* 3. missense-mutation. The whole genome has to be evaluated from the start with createGenome, save the generate_DNA part. The
	*    color of that pixie should deviate more from the pixieStrand
	* 4. silent mutation: mutation in the snk/src ID, but still coding for the same gene. In this case bwd_adj AND topoOrder can stay
	*	 the same, only DNA has to be updated
	* FUNCTION: inheritGenome(), mutateDNA(), generateSimilarColor()
	* Then place the Pixie into the new World and add its other components (as above)
	* FUNCTION: inheritPixie() as a caller of the above functions \selectPixies
	*/
	void inheritPixie(World* newW, const Genome oldGenome) {
		Entity newPixie = newW->pixie_em.create();

		int randX;
		int randY;
		while (true) {
			randX = randomengine->getRandomIntCustom(0, gridsizeX - 1);
			randY = randomengine->getRandomIntCustom(0, gridsizeY - 1);
			if (newW->getGridCell(randY, randX) == EMPTY) { break; }
		}
		newW->Pos.add(newPixie, Position{ randY, randX });
		newW->setGridCell(newW->Pos.get(newPixie), newPixie);

		newW->facing.add(newPixie, 0.0);

		newW->move_urge.add(newPixie, MoveUrge{ 0, 0 });

		newW->brainstate.add(newPixie, BrainState{}); // empty brain 

		newW->fitness.add(newPixie, 1.0);

		newW->searchRadius.add(newPixie, defaultSearchRadius);

		newW->pixie_neighbourhood.add(newPixie, Neighbourhood{});

		// last step: inherit the genome
		newW->PixieGenomes.add(newPixie, inheritGenome(newW, newPixie, oldGenome));
	}

	void newGeneration(World* newW) {
		for (int i = 0; i < numberOfPixies; i++) {
			spawnPixie(newW);
		}
	}
	void newGeneration(World* newW, const std::vector<Genome>& nextMetagenome) {
		// for each genome in nextMetagenome, spawn a new pixie
		for (int i = 0; i < numberOfPixies; i++) {
			inheritPixie(newW, nextMetagenome.at(i)); // analogous to spawnPixie; but with predefined Genomes
		}

	}
	void eachSimStep(World* w, int gen, int age) {


		// this can be multithreaded
		const auto& entity_list = w->PixieGenomes.get_entities();
#pragma omp parallel for //schedule(static) / schedule(dynamic)
		for (const Entity& p : entity_list) {
			execute_staticBrain(w, p);
		}

		for (const Entity& p : w->queueForMove) {
			executeMove(w, p);
		}
		w->queueForMove.clear();
		w->pixie_neighbourhood.for_each([&](Entity e, Neighbourhood c) { c.up_to_date = false; });
		 
		//(pheromoneDecay)
		
		//render simstep if correct gen
		if (shouldCreateGIF(gen)) {
			writeGIFdata(gen, w);
		}
	}
	void evaluateFitness(World* w) {
		funcTableSelCrit[selectionCriterium](w);
	}
	std::vector<Genome> select(World* w) {
		std::vector<Genome> selected_genomes;
		selected_genomes.reserve(numberOfPixies);

		while (selected_genomes.size() < numberOfPixies) { // this should stop when the size of numberOfPixies is reached
			Entity rand_Entity = w->fitness.random_entity();
			if (w->fitness.get(rand_Entity) > randomengine->getRandom01()) {
				Genome pixie_gnm = w->genome.get(w->PixieGenomes.get(rand_Entity)); 
				selected_genomes.push_back(pixie_gnm);
			}
		}

		return selected_genomes;
	}


	void simulateGenerations() { // overload with NO starting metagenome (everything gets generated from scratch)

		// before all generations:

		// check if too many pixies for grid
		if (gridsizeX * gridsizeY < numberOfPixies) {
			std::cerr << "too many pixies for the grid!";
		}

		// Container for genomes for the next generation:
		//ComponentStorage<Genome> nextMetagenome;
		std::vector<Genome> nextMetagenome; 
		nextMetagenome.reserve(numberOfPixies);


		//std::unique_ptr<World> oldWorld = nullptr; //empty world adress // we need either a dummy world or a placeholder adress
		// for each generation:
		for (int gen = 1; gen <= numberOfGenerations; gen++) {

			std::cout << "generation " << gen << "\n";
			//create new World
			World newWorld; // on stack // pixie and brain Template EntityManagers are automatically created with the World
			World* newWorld_ptr = &newWorld;
			
			// set environment //

			// if first world:
			if (gen == 1) {
				// spawn Pixies with new Genomes
				newGeneration(newWorld_ptr);
			}
			// else if succeding world:
			else {
				// spawn new Pixies but inherit Genomes
				newGeneration(newWorld_ptr, nextMetagenome);
			}
			nextMetagenome.clear();
			
			// simulate simSteps
			//newWorld_ptr->printGrid();
			for (int i = 0; i < numberOfSimSteps; i++) {

				//generationAge = i; // NOT THREAD SAFE
				eachSimStep(newWorld_ptr, gen, i);

				//if (fitnessUpdates == "every") {
				//	evaluateFitness(newWorld);
				//	//applySelectionCriteria(newWorld); // (only for soft selection criteria)
				//}
			}
			//newWorld_ptr->printGrid();

			// apply Selection
			evaluateFitness(newWorld_ptr);
			//newWorld_ptr->fitness.for_each([&](Entity e, float c) { std::cout << e << " " << c << ", " << newWorld_ptr->fitness.has(e) << "\n"; });
			nextMetagenome = select(newWorld_ptr);
			
			int fit_pixies = 0;
			newWorld_ptr->fitness.for_each([&](Entity e, float c) {if (c > 0) { fit_pixies++; }; });
			if (fit_pixies == 0) {
				std::cout << "total extinction!!\n";
				break;
			}
			
			// select pixies to be reproduced by fitness and fill numPixies worth of Genomes into the nextMetagenome Object
		} // end for gen

		//if (createGIF == "every" || createGIF == "selected" || createGIF == "last") { renderGIFs(); }
	}
	void simulateGenerations(const std::vector<Genome>& startingMetagenome) { // overload WITH starting Metagenome. Keep function body updated!!

		// before all generations:

		// check if too many pixies for grid
		if (gridsizeX * gridsizeY < numberOfPixies) {
			std::cerr << "too many pixies for the grid!";
		}

		// Container for genomes for the next generation:
		//ComponentStorage<Genome> nextMetagenome;
		std::vector<Genome> nextMetagenome;
		nextMetagenome.reserve(numberOfPixies);

		nextMetagenome = startingMetagenome; // for first generation //////////////////////////////////////////
		

		//std::unique_ptr<World> oldWorld = nullptr; //empty world adress // we need either a dummy world or a placeholder adress
		// for each generation:
		for (int gen = 1; gen <= numberOfGenerations; gen++) {

			std::cout << "generation " << gen << "\n";
			//create new World
			World newWorld; // on stack // pixie and brain Template EntityManagers are automatically created with the World
			World* newWorld_ptr = &newWorld;

			// set environment //

			// if first world:
			if (gen == 1) {
				// spawn Pixies with new Genomes
				newGeneration(newWorld_ptr, startingMetagenome); //////////////////////////////////////////////
			}
			// else if succeding world:
			else {
				// spawn new Pixies but inherit Genomes
				newGeneration(newWorld_ptr, nextMetagenome);
			}
			nextMetagenome.clear();

			// simulate simSteps
			for (int i = 0; i < numberOfSimSteps; i++) {

				//generationAge = i;
				eachSimStep(newWorld_ptr, gen, i);

				//if (fitnessUpdates == "every") {
				//	evaluateFitness(newWorld);
				//	//applySelectionCriteria(newWorld); // (only for soft selection criteria)
				//}
			}

			evaluateFitness(newWorld_ptr);
			//if (!chooseParentsByFitness) { // this means that pixies should be killed before choosing pixies to reproduce
			//	applySelectionCriteria(newWorld);
			// or select();
			//}
			int fit_pixies = 0;
			newWorld_ptr->fitness.for_each([&](Entity e, float c) {if (c > 0) { fit_pixies++; }; });
			if (fit_pixies == 0) {
				std::cout << "total extinction!!\n";
				break;
			}

			// select pixies to be reproduced by fitness and fill numPixies worth of Genomes into the nextMetagenome Object

		}
	}

}