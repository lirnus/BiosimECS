#include "environment.h"

#pragma once

namespace bs {
	// lookup table for selection criteria, like neuron functions	

	// lookup-Table for Neuron functions
	using SelectionFunc = void(*)(World* w);
	extern std::array<SelectionFunc, NUM_SELCRIT> funcTableSelCrit;
	void initSelectionFuncTable();

	// selection functions
	void no_selection(World* w);
	void killRightHalf(World* w);
	void goLeft_Gradient(World* w);
}