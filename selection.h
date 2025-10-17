#include "environment.h"

#pragma once

namespace bs {
	// lookup table for selection criteria, like neuron functions

	// enum for SelCrit? just to keep track of the number of selection criteria and insert in initFuncTable
	enum SelCrit : uint8_t {
		NO_SELECTION,
		KILLRIGHTHALF,
		NUM_SELCRIT
	};

	// lookup-Table for Neuron functions
	using SelectionFunc = void(*)(World* w);
	extern std::array<SelectionFunc, NUM_SELCRIT> funcTableSelCrit;
	void initSelectionFuncTable();

	// selection functions
	void no_selection(World* w);
	void killRightHalf(World* w);
}