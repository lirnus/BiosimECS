#include "global_params.h"

#include <chrono>
#include <fstream>
#include <filesystem>
//#include <iomanip>
#include <sstream>

namespace bs {
	// initialize Param structs
	WorldParams* worldParams = new bs::WorldParams();
	SimulationParams* simParams = new bs::SimulationParams();
	PopulationParams* popParams = new bs::PopulationParams();
	PixieParams* pixParams = new bs:: PixieParams();
	ActiveNeurons* activeNeurons = new bs::ActiveNeurons();
	AnalyticsParams* analParams = new bs::AnalyticsParams();
	RenderParams* renderParams = new bs::RenderParams();
	BaseDir* basedir_struct = new bs::BaseDir();
	RngParams* rngParams = new RngParams();
	
	// random engine
	Random* randomengine = new bs::Random();

	// neuron Classes vector tracking how many neurons of each class are active
	std::vector<int> neuronClasses{};


	void initParameters(const std::string& filename) {
		// read through simconfig.ini and extract all relevant parameters
		std::ifstream file(filename);
		if (!file) throw std::runtime_error("Cannot open config file");
		
		std::string section;
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == '#') continue; // ignore empty lines or comment lines
			
			if (line[0] == '[') {// new section
				section = line;
				continue;
			}

			auto pos = line.find('=');
			if (pos == std::string::npos) continue;
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			
			// trim spaces
			key.erase(0, key.find_first_not_of(" \t"));
			key.erase(key.find_last_not_of(" \t") + 1);
			value.erase(0, value.find_first_not_of(" \t"));
			value.erase(value.find_last_not_of(" \t") + 1);

			if (section == "[world parameters]") {
				if (key == "gridsizeX") worldParams->gridSizeX = std::stoi(value);
				else if (key == "gridsizeY") worldParams->gridSizeY = std::stoi(value);
				else if (key == "numberOfGenes") worldParams->numberOfGenes = std::stoi(value);
				else if (key == "numberOfPixies") worldParams->numberOfPixies = std::stoi(value);
				else if (key == "numberOfGenerations") worldParams->numberOfGenerations = std::stoi(value);
				else if (key == "numberOfSimSteps") worldParams->numberOfSimSteps = std::stoi(value);
				else if (key == "selectionCriterium") worldParams->selectionCriterium = std::stoi(value);
				else if (key == "Barriers_Key") worldParams->Barriers_Key = std::stoi(value);
				else if (key == "Interactives_Key") worldParams->Interactives_Key = std::stoi(value);
			}
			else if (section == "[simulation settings]") {
				if (key == "startingPopulation") simParams->startingPopulation = (value == "true" || value == "True" || value == "T");
				else if (key == "startingPop_path") simParams->startingPop_path = value;
			}
			else if (section == "[continuous simulation parameters]") {

			}
			else if (section == "[population parameters]") {
				if (key == "blockedByOtherPixies") popParams->blockedByOtherPixies = (value == "true" || value == "True" || value == "T");
				else if (key == "pixies_per_genome") popParams->pixies_per_genome = std::stoi(value);
			}
			else if (section == "[pixie parameters]") {
				if (key == "mutationRate") pixParams->mutationRate = std::stof(value);
				else if (key == "weight_factor") pixParams->weight_factor = std::stof(value);
				else if (key == "defaultSearchRadius") pixParams->defaultSearchRadius = std::stof(value);
			}
			else if (section == "[neurons]") {
				//s
				if (key == "xPos") activeNeurons->xPos = (value == "true" || value == "True" || value == "T");
				else if (key == "yPos") activeNeurons->yPos = (value == "true" || value == "True" || value == "T");
				else if (key == "popDensityFwd") activeNeurons->popDensityFwd = (value == "true" || value == "True" || value == "T");
				else if (key == "age") activeNeurons->age = (value == "true" || value == "True" || value == "T");
				//i
				else if (key == "numberOfInternals") activeNeurons->numInternals = std::stoi(value);
				// a
				else if (key == "moveW") activeNeurons->moveW = (value == "true" || value == "True" || value == "T");
				else if (key == "moveE") activeNeurons->moveE = (value == "true" || value == "True" || value == "T");
				else if (key == "moveN") activeNeurons->moveN = (value == "true" || value == "True" || value == "T");
				else if (key == "moveS") activeNeurons->moveS = (value == "true" || value == "True" || value == "T");
			}
			else if (section == "[analytics]") {
				if (key == "calc_pop_stats") analParams->calc_pop_stats = (value == "true" || value == "True" || value == "T");
				else if (key == "save_metagenome") analParams->save_metagenome = value;
				else if (key == "saveMetagenomeEvery") analParams->saveMetagenomeEvery = std::stoi(value);
				else if (key == "saveMetagenomeFor") {
					std::stringstream value_stream;
					std::string gen;
					while (std::getline(value_stream, gen, ',')) {
						analParams->saveMetagenomeFor.push_back(std::stoi(gen));
					}
				}
			}
			else if (section == "[render settings]") {
				if (key == "createGIF") renderParams->createGIF = value;
				else if (key == "GIF_resolution") renderParams->GIF_resolution = std::stoi(value);
				else if (key == "GIF_frameduration") renderParams->GIF_frameduration = std::stoi(value);
				else if (key == "createGIFevery") renderParams->createGIFevery = std::stoi(value);
				else if (key == "createGIFfor") {
					std::stringstream value_stream;
					std::string gen;
					while (std::getline(value_stream, gen, ',')) {
						renderParams->createGIFfor.push_back(std::stoi(gen));
					}
				}
				else if (key == "color_variation") renderParams->color_variation = std::stoi(value);
			}
			else if (section == "[file system paths etc.]") {
				if (key == "baseDir") basedir_struct->baseDir = value;
			}
			else if (section == "[random number generator]") {
				if (key == "deterministic") rngParams->deterministic = (value == "true" || value == "True" || value == "T");
				else if (key == "seeed") rngParams->seeed = std::stoi(value);
			}
			
		}//end of while getline

	}//end of initParameters

	//runtime variables are initialized here

	int generationAge{};

	// folder directories
	std::string folder_dir{};
	std::string cwd_str{};

	// timekeeping
	double simulationTime{};
	double renderingTime{};
	

	void initFolderDir() {
		std::string filenameHeader = "BiosimECS";

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
#ifdef _WIN32
		localtime_s(&tm, &t);  // Windows
#else
		localtime_r(&t, &tm);  // Linux/Unix
#endif
		// Zeit als String formatieren
		std::ostringstream oss;
		oss << filenameHeader << "_"
			<< std::put_time(&tm, "%Y-%m-%d_%H-%M");

		std::string folderName = oss.str();

		// Im aktuellen Arbeitsverzeichnis erstellen
		namespace fs = std::filesystem;
		fs::path folderPath = fs::current_path() / folderName;

		try {
			if (fs::create_directory(folderPath)) {
				std::cout << "Ordner erstellt: " << folderPath << "\n";
			}
			else {
				std::cout << "Ordner konnte nicht erstellt werden (existiert evtl. schon?)\n";
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Fehler beim Erstellen: " << e.what() << "\n";
		}

		std::string folderPathStr = folderPath.string();

		folder_dir = folderPathStr;

		// get cwd and save the folder_dir in a varíable and atext file 
		fs::path cwd = std::filesystem::current_path();
		std::cout << "Current working directory: " << cwd << std::endl;
		cwd_str = cwd.string();
		/*std::ostringstream oss_cwd;
		oss_cwd << cwd_str*/
		fs::path filename = cwd / "folderdir.txt";

		std::ofstream file(filename);
		if (!file.is_open()) {
			std::cout << "could not open folderdir.txt file :(\n";
			return;
		}

		file << folder_dir;

		file.close();
	}
	void initFolderDir(std::string baseDir) {
		std::string filenameHeader = "BiosimECS";

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm{};
#ifdef _WIN32
		localtime_s(&tm, &t);  // Windows
#else
		localtime_r(&t, &tm);  // Linux/Unix
#endif
		// Zeit als String formatieren
		std::ostringstream oss;
		oss << filenameHeader << "_"
			<< std::put_time(&tm, "%Y-%m-%d_%H-%M");

		std::string folderName = oss.str();

		// Im übergebenen Verzeichnis erstellen
		namespace fs = std::filesystem;
		fs::path folderPath = fs::path(baseDir) / folderName;

		try {
			if (fs::create_directory(folderPath)) {
				std::cout << "Ordner erstellt: " << folderPath << "\n";
			}
			else {
				std::cout << "Ordner konnte nicht erstellt werden (existiert evtl. schon?)\n";
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Fehler beim Erstellen: " << e.what() << "\n";
		}

		std::string folderPathStr = folderPath.string();

		folder_dir = folderPathStr;

		// get cwd and save the folder_dir in a varíable and a text file 
		fs::path cwd = std::filesystem::current_path();
		std::cout << "Current working directory: " << cwd << std::endl;
		cwd_str = cwd.string();
		/*std::ostringstream oss_cwd;
		oss_cwd << cwd_str*/
		fs::path filename = cwd / "folderdir.txt";

		std::ofstream file(filename);
		if (!file.is_open()) {
			std::cout << "could not open folderdir.txt file :(\n";
			return;
		}

		file << folder_dir;

		file.close();
	}

}
