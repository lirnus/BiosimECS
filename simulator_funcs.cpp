#include "simulator_funcs.h"

#include <algorithm>
#include <memory>
#include <iostream>
#include <array>
#include <fstream>
#include <sstream>

#include <omp.h> // Multithread

namespace bs {

	void initializePixie(World* w, Entity& newPixie) {
		int randX;
		int randY;
		while (true) {
			randX = randomengine->getRandomIntCustom(0, worldParams->gridSizeX - 1);
			randY = randomengine->getRandomIntCustom(0, worldParams->gridSizeY - 1);
			if (w->getGridCell(randY, randX) == EMPTY) { break; }
		}
		w->Pos.add(newPixie, Position{ randY, randX });
		w->setGridCell(w->Pos.get(newPixie), newPixie);

		w->facing.add(newPixie, 0.0);

		w->move_urge.add(newPixie, MoveUrge{ 0, 0 });

		w->brainstate.add(newPixie, BrainState{}); // empty brain 

		w->fitness.add(newPixie, 1.0);

		w->searchRadius.add(newPixie, pixParams->defaultSearchRadius);

		w->pixie_neighbourhood.add(newPixie, Neighbourhood{});
	}

	void spawnPixie(World* w) {
		// create a new Pixie entity
		Entity newPixie = w->pixie_em.create();

		// initialize its components, place on grid
		initializePixie(w, newPixie);

		// create a new genome
		w->PixieGenomes.add(newPixie, createGenome(w, newPixie));
			
	}
	
	void inheritPixie(World* newW, const Genome& oldGenome) {
		// create a new Pixie entity
		Entity newPixie = newW->pixie_em.create();

		// initialize its components, place on grid
		initializePixie(newW, newPixie);

		// inherit the genome
		newW->PixieGenomes.add(newPixie, inheritGenome(newW, newPixie, oldGenome));
	}
	void inheritPixie(World* newW, const startingGenome& strt_gnm) {
		// create a new Pixie entity
		Entity newPixie = newW->pixie_em.create();

		// initialize its components, place on grid
		initializePixie(newW, newPixie);

		// inherit the genome
		newW->PixieGenomes.add(newPixie, inheritGenome_fromDNA(newW, newPixie, strt_gnm));
	}

	void newGeneration(World* newW) {
		for (int i = 0; i < worldParams->numberOfPixies; i++) {
			spawnPixie(newW);
		}
	}
	void newGeneration(World* newW, const std::vector<Genome>& nextMetagenome) {
		// for each genome in nextMetagenome, spawn a new pixie
		for (int i = 0; i < worldParams->numberOfPixies; i++) {
			inheritPixie(newW, nextMetagenome.at(i)); // analogous to spawnPixie; but with predefined Genomes
		}

	}
	void newGeneration_fromTextfile(World* newW) {
		const std::vector<startingGenome> startingPop = readMetagenome();

		for (int i = 0; i < worldParams->numberOfPixies; i++) {
			inheritPixie(newW, startingPop.at(i));
		}
	}


	void eachSimStep(World* w, int gen, int age) {

		// this can be multithreaded
		const auto& entity_list = w->PixieGenomes.get_entities();
#pragma omp parallel for //schedule(static) / schedule(dynamic)
		for (const Entity& p : entity_list) {
			execute_staticBrain(w, p);
		}
// end of parallel loop

		for (const Entity& p : w->queueForMove) {
			executeMove(w, p);
		}
		w->queueForMove.clear();
		w->pixie_neighbourhood.for_each([&](Entity e, Neighbourhood c) { c.up_to_date = false; });
		 
		// DEBUG
		w->printPheromoneGrid();
		//pheromoneDecay
		w->pheromoneDecay();
		
		//render simstep if correct gen
		if (shouldCreateGIF(gen)) {
			writeGIFdata(gen, w);
		}
	}

	void evaluateFitness(World* w) {
		funcTableSelCrit[worldParams->selectionCriterium](w);
	}

	std::vector<Genome> select(World* w) {
		std::vector<Genome> selected_genomes;
		selected_genomes.reserve(worldParams->numberOfPixies);

		size_t numPixies = numPixies = worldParams->numberOfPixies;
		while (selected_genomes.size() < numPixies) { // this should stop when the size of numberOfPixies is reached
			Entity rand_Entity = w->fitness.random_entity();
			if (w->fitness.get(rand_Entity) > randomengine->getRandom01()) {
				selected_genomes.push_back(
					std::move(w->genome.get(w->PixieGenomes.get(rand_Entity))) // move is way faster than copy for large objects
				);
			}
		}

		return selected_genomes;
	}


	void simulateGenerations() { // overload with NO starting metagenome (everything gets generated from scratch)

		int numGens = worldParams->numberOfGenerations;
		int numSimSteps = worldParams->numberOfSimSteps;

		// before all generations:

		// check if too many pixies for grid
		if (worldParams->gridSizeX * worldParams->gridSizeY < worldParams->numberOfPixies) {
			std::cerr << "too many pixies for the grid!";
		}

		// Container for genomes for the next generation:
		//ComponentStorage<Genome> nextMetagenome;
		std::vector<Genome> nextMetagenome; 
		nextMetagenome.reserve(worldParams->numberOfPixies);


		//std::unique_ptr<World> oldWorld = nullptr; //empty world adress // we need either a dummy world or a placeholder adress
		// for each generation:
		for (int gen = 1; gen <= numGens; gen++) {

			std::cout << "generation " << gen << "\n";
			//create new World
			World newWorld(worldParams); // on stack // pixie and brain Template EntityManagers are automatically created with the World
			World* newWorld_ptr = &newWorld;
			
			// set environment
			setEnvironment(newWorld_ptr);

			// if first world:
			if (gen == 1) {
				if (simParams->startingPopulation) {
					//readMetagenome(newWorld_ptr, startingPop_filepath)
				}
				else {
					// spawn Pixies with new Genomes
					newGeneration(newWorld_ptr);
				}
			}// else if succeding world:
			else {
				// spawn new Pixies but inherit Genomes
				newGeneration(newWorld_ptr, nextMetagenome);
			}
			nextMetagenome.clear();
			
			// simulate simSteps
			//newWorld_ptr->printGrid();
			for (int i = 0; i < numSimSteps; i++) {

				generationAge = i; 
				eachSimStep(newWorld_ptr, gen, i);

				//if (fitnessUpdates == "every") {
				//	evaluateFitness(newWorld);
				//	//applySelectionCriteria(newWorld); // (only for soft selection criteria)
				//}
			}
			//newWorld_ptr->printGrid();


			// apply Selection
			evaluateFitness(newWorld_ptr);

			// if needed, save metagenome of this generation
			if (shouldSaveMetagenome(gen)) saveMetagenome(newWorld_ptr, gen);

			// if needed, save populations stats
			if (analParams->calc_pop_stats) writePopulationStats(newWorld_ptr, gen);
			
			// select pixies to be reproduced by fitness and fill numPixies worth of Genomes into the nextMetagenome Object
			nextMetagenome = select(newWorld_ptr);
			
			int fit_pixies = 0;
			newWorld_ptr->fitness.for_each([&](Entity e, float c) {if (c > 0) { fit_pixies++; }; });
			if (fit_pixies == 0) {
				std::cout << "total extinction!!\n";
				break;
			}
			
		} // end for gen

	}// end simulateGenerations
	
}