#include "simulator_funcs.h"

#include <algorithm>
#include <memory>
#include <iostream>

namespace bs {
	void spawnPixie(std::unique_ptr<World> w) {
		// create a new Pixie entity and place it on the grid
		Entity newPixie = w->pixie_em.create();

		int randX;
		int randY;
		while (true) {
			randX = randomengine->getRandomIntCustom(0, gridsizeX - 1);
			randY = randomengine->getRandomIntCustom(0, gridsizeY - 1);
			if (w->getGridCell(randY, randX) == EMPTY) { break; }
		}
		Pos.add(newPixie, Position{ randY, randX });
		w->setGridCell(Pos.get(newPixie), newPixie);

		uint8_t randR = randomengine->getRandomIntCustom(0, 255);
		uint8_t randG = randomengine->getRandomIntCustom(0, 255);
		uint8_t randB = randomengine->getRandomIntCustom(0, 255);
		Col.add(newPixie, Color{ randR, randG, randB });

		facing.add(newPixie, 0);

		move_urge.add(newPixie, MoveUrge{ 0, 0 });

		brainstate.add(newPixie, BrainState{}); // empty brain 
			
	}


	void simulateGenerations(const std::optional<ComponentStorage<Genome>>& startingMetagenome = std::nullopt) {

		// before all generations:

		// check if too many pixies for grid
		if (gridsizeX * gridsizeY < numberOfPixies) {
			std::cerr << "too many pixies for the grid!";
		}

		std::unique_ptr<World> oldWorld = nullptr; //empty world adress // we need either a dummy world or a placeholder adress
		// for each generation:
		for (int gen = 1; gen <= numberOfGenerations; gen++) {

			std::cout << "generation " << gen << "\n";
			//create new World
			// pixie and brain Template EntityManagers are automatically created with the World
			
			std::unique_ptr<World> newWorld = std::make_unique<World>(); // smart pointers get deleted when they are out of scope/dangling
			// set environment //

			// if first world:
			if (gen == 1 && oldWorld == nullptr) {
				// spawn Pixies with new Genomes
				newGeneration(newWorld, startingMetagenome);
			}
			// else if succeding world:
			else if (oldWorld != nullptr) {
				// spawn new Pixies but inherit Genomes
				newGeneration(newWorld, oldWorld);
			}
			
			// simulate simSteps
			for (int i = 0; i < numberOfSimSteps; i++) {

				//generationAge = i;
				eachSimStep(newWorld, gen);

				if (fitnessUpdates == "every") {
					evaluateFitness(newWorld);
					//applySelectionCriteria(newWorld); // (only for soft selection criteria)
				}
			}

			evaluateFitness(newWorld);
			//if (!chooseParentsByFitness) { // this means that pixies should be killed before choosing pixies to reproduce
			//	applySelectionCriteria(newWorld);
			//}
			int fit_pixies = 0;
			fitness.for_each([&](Entity e, float c) {if (c > 0) { fit_pixies++; }; });
			if (fit_pixies == 0) {
				std::cout << "total extinction!!\n";
				break;
			}
			
			oldWorld = std::move(newWorld);
			// clear all the pixie component storages: Pos, Col, facing, move_urge, BrainState, fitness
			Pos.clear_all();
			Col.clear_all();
			facing.clear_all();
			move_urge.clear_all();
			brainstate.clear_all();
			// clear all the genome component storages: topoOrder, bwd_adjacency

		}
	}

}