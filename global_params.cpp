#include "global_params.h"

namespace bs {
	// world parameters
	int gridsizeY = 30;
	int gridsizeX = 30;
	int numberOfGenes = 16;
	int numberOfPixies = 200;
	int numberOfGenerations = 2;
	int numberOfSimSteps = 40;
	/*std::vector<std::string> selectionCriteria = { "killRightHalf" };
	std::string EnvironmentKey = "empty";*/



	// random engine
	Random* randomengine = new bs::Random();
}