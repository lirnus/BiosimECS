#pragma once
#include "World.h" // incl ecs_framework, components, global_params

namespace bs {

	// actions
	void executeMove(World* w, const Entity& p);

	// surroundings
	const Neighbourhood& scanNeighbourhood(World* w, const Entity& p);

	// math
	const Position& getRelativePosition(const int r, const int c, const Position& ownPos);

	float getRelAngle(const int relY, const int relX);
	float getRelAngle(const Position& relPos);

	//float getNormalizedDirection(const float angle); // + overloads with Position, vector?
	
	float calcEuclidianDistance(const int x, const int y);
	float calcEuclidianDistance(const Position& relPos);

	bool isInSearchRadius(const int x, const int y, const World* w, const Entity& p);
	bool isInSearchRadius(const Position& relPos, const World* w, const Entity& p);

}