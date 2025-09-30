#include <iostream>
#include <chrono>

#include "components.h"
#include "World.h"
#include "simulator_funcs.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";
	auto start = std::chrono::high_resolution_clock::now();

	World myWorld;
	World* world_ptr = &myWorld;

	/*EntityManager brain_templates;

	ComponentStorage<std::vector<uint32_t>> bt_comp1;

	Entity bt1 = brain_templates.create();
	Entity bt2 = brain_templates.create();

	bt_comp1.add(bt1, { 83274, 876432 });
	bt_comp1.add(bt2, { 12345, 555466 });

	Pos.for_each([&](Entity e, Position& c) {
		myWorld.setGridCell(c, e);
		});*/

	for (int i = 0; i < numberOfPixies; i++) {
		spawnPixie(world_ptr);
	}

	myWorld.printGrid();
	
	/*bt_comp1.for_each([&](Entity e, std::vector<uint32_t>& c) {
		std::cout << "genome " << e << " genes = ";
		for (auto gene : c) { std::cout << gene << " "; }
		std::cout << "\n";
	});*/

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "time elapsed: " << elapsed.count() << " seconds\n";
	std::cout << "program finished\n";

	return 0;
}