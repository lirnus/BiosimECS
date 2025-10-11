#pragma once
#include "World.h" // incl ecs_framework, components, global_params

namespace bs {

	// actions
	void executeMove(World* w, const Entity& p);

	// surroundings
	Neighbourhood scanNeighbourhood(const World* w, const Entity& p);

	// math
	const Position& getRelativePosition(const int r, const int c, const Position& ownPos);
	
	float calcEuclidianDistance(const int x, const int y);
	float calcEuclidianDistance(const Position& relPos);

	bool isInSearchRadius(const int x, const int y, const World* w, const Entity& p);
	bool isInSearchRadius(const Position& relPos, const World* w, const Entity& p);

}