#include "simulator_funcs.h"
#include "Genome.h" //incl Neurons, World, ecs_framework -> global_params

#include <algorithm>
#include <memory>
#include <iostream>

namespace bs {
	void spawnPixie(World* w) {
		// create a new Pixie entity and place it on the grid
		Entity newPixie = w->pixie_em.create();

		int randX;
		int randY;
		while (true) {
			randX = randomengine->getRandomIntCustom(0, gridsizeX - 1);
			randY = randomengine->getRandomIntCustom(0, gridsizeY - 1);
			if (w->getGridCell(randY, randX) == EMPTY) { break; }
		}
		w->Pos.add(newPixie, Position{ randY, randX });
		w->setGridCell(w->Pos.get(newPixie), newPixie);

		/*uint8_t randR = randomengine->getRandomIntCustom(0, 255);
		uint8_t randG = randomengine->getRandomIntCustom(0, 255);
		uint8_t randB = randomengine->getRandomIntCustom(0, 255);
		w->Col.add(newPixie, Color{ randR, randG, randB });*/		// this comes when creating a genome

		w->facing.add(newPixie, 0.0);

		w->move_urge.add(newPixie, MoveUrge{ 0, 0 });

		w->brainstate.add(newPixie, BrainState{}); // empty brain 

		w->fitness.add(newPixie, 1.0);

		// create a genome:
		w->Genomes.add(newPixie, createGenome(w, newPixie));
			
	}
	

	void inheritPixie(World& newW, Entity genomeID) {

	}
	void eachSimStep(World*, int gen) {

	}
	void newGeneration(World* newW, const std::optional<ComponentStorage<Genome>>& genome) {

	}
	void newGeneration(World* newW, const ComponentStorage<Genome>& genome) {

	}
	void evaluateFitness(World* w) {

	}


	void simulateGenerations(const std::optional<ComponentStorage<Genome>>& startingMetagenome) {

		// before all generations:

		// check if too many pixies for grid
		if (gridsizeX * gridsizeY < numberOfPixies) {
			std::cerr << "too many pixies for the grid!";
		}

		// Container for genomes for the next generation:
		ComponentStorage<Genome> nextMetagenome;

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
				newGeneration(newWorld_ptr, startingMetagenome);
			}
			// else if succeding world:
			else {
				// spawn new Pixies but inherit Genomes
				newGeneration(newWorld_ptr, nextMetagenome);
			}
			nextMetagenome.clear_all();
			
			// simulate simSteps
			for (int i = 0; i < numberOfSimSteps; i++) {

				//generationAge = i;
				eachSimStep(newWorld_ptr, gen);

				//if (fitnessUpdates == "every") {
				//	evaluateFitness(newWorld);
				//	//applySelectionCriteria(newWorld); // (only for soft selection criteria)
				//}
			}

			evaluateFitness(newWorld_ptr);
			//if (!chooseParentsByFitness) { // this means that pixies should be killed before choosing pixies to reproduce
			//	applySelectionCriteria(newWorld);
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