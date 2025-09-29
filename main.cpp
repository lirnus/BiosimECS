#include <iostream>

#include "components.h"
#include "World.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";

	World myWorld;

	EntityManager pixie;
	EntityManager brain_templates;

	ComponentStorage<std::vector<uint32_t>> bt_comp1;

	Entity p1 = pixie.create();
	Entity p2 = pixie.create();
	Entity p3 = pixie.create();

	Entity bt1 = brain_templates.create();
	Entity bt2 = brain_templates.create();

	Pos.add(p1, Position{ 1, 0 });
	Pos.add(p2, Position{ 3, 2 });
	Pos.add(p3, Position{ 9, 9 });

	bt_comp1.add(bt1, { 83274, 876432 });
	bt_comp1.add(bt2, { 12345, 555466 });

	Pos.for_each([&](Entity e, Position& c) {
		myWorld.setGridCell(c, e);
		});

	myWorld.printGrid();
	
	bt_comp1.for_each([&](Entity e, std::vector<uint32_t>& c) {
		std::cout << "genome " << e << " genes = ";
		for (auto gene : c) { std::cout << gene << " "; }
		std::cout << "\n";
	});

	std::cout << "program finished\n";

	return 0;
}