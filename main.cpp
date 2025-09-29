#include <iostream>

#include "ecs_framework.h"


int main() {
	using namespace bs;

	std::cout << "program is running...\n\n";

	EntityManager pixie;
	EntityManager brain_templates;

	ComponentStorage<int> comp1;

	Entity p1 = pixie.create();
	Entity p2 = pixie.create();
	Entity p3 = pixie.create();

	comp1.add(p1, 3);
	comp1.add(p2, 0);

	comp1.for_each([&](Entity e, int& c) {
		std::cout << "Entity " << e << " number = " << c << "\n";
	});

	std::cout << "program finished\n";

	return 0;
}