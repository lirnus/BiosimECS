#include "analysis.h"

#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <charconv>

namespace bs {

	// metadata
	void saveMetaData() {
		std::ostringstream oss;
		oss << bs::folder_dir << "/metadata.txt";
		std::string fileName = oss.str();

		// open
		std::ofstream file(fileName);
		if (!file.is_open()) {
			std::cout << "could not open metadata file :(\n";
			return;
		}
		// get current time
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
#ifdef _WIN32
		localtime_s(&tm, &t);  // Windows
#else
		localtime_r(&t, &tm);  // Linux/Unix
#endif

		// write
		file << "+++ Pixies Evolutionary Simulation +++\n\n";

		file << "experiment from " << std::put_time(&tm, "%m.%d.%Y %H:%M") << "\n";

		file << "Time elapsed during simulation: " << simulationTime << " sec. ";
		file << "Time elapsed during rendering: " << renderingTime << " sec.\n";

		file << "\nWorld parameters:\n";
		file << "gridsizeX: "			<< gridsizeX				<< "\n";
		file << "gridsizeY: "			<< gridsizeY				<< "\n";
		file << "numberOfPixies: "		<< numberOfPixies			<< "\n";
		file << "numberOfGenes: "		<< numberOfGenes			<< "\n";
		file << "numberOfGenerations: "	<< numberOfGenerations		<< "\n";
		file << "numberOfSimSteps: "	<< numberOfSimSteps			<< "\n";
		file << "numberOfPixies: "		<< numberOfPixies			<< "\n";
		file << "numberOfPixies: "		<< numberOfPixies			<< "\n";
		file << "numberOfPixies "		<< numberOfPixies			<< "\n";
		file << "selectionCriterium: "	<< selCrit_printable()		<< "\n";
		file << "Barriers_Key: "		<< barriers_printable()		<< "\n";
		file << "Interactives_Key: "	<< interactives_printable() << "\n";

		file << "\nSimulator settings:\n";
		file << "startingPopulation: "	<< (startingPopulation ? "true" : "false") << "\n";
		file << "startingPop_path: "	<< startingPop_path			<< "\n";

		file << "\nContinuous simulator settings:\n";

		file << "\nPopulation settings:\n";
		file << "blockedByOtherPixies: " << (blockedByOtherPixies ? "true" : "false") << "\n";
		file << "pixies_per_genome: "	<< pixies_per_genome		<< "\n";

		file << "\nPixie parameters:\n";
		file << "mutationRate: "		<< mutationRate				<< "\n";
		file << "weight_factor: "		<< weight_factor			<< "\n";
		file << "defaultSearchRadius: "	<< defaultSearchRadius		<< "\n";

		file << "\nReproducibility:\n";
		file << "deterministic: " << (deterministic ? "true" : "false") << "\n";
		file << "random seed: " << seeed << "\n";

		file << "\nEnabled Neurons:\n";
		for (int i = 0; i < NeuronTypes::NUM_NEURONS; i++) {
			file << neurons_printable(i) << "\n";
		}



		// close
		file.close();
	}
	std::string_view selCrit_printable() { // HAS TO BE UPDATED MANUALLY 
		uint8_t key = selectionCriterium;

		switch (key) {
		case NO_SELECTION:
			return "NO_SELECTION";
		case KILLRIGHTHALF:
			return "KILLRIGHTHALF";
		
		default:
			return "n/a";
		}
		
	}
	std::string_view barriers_printable() { // HAS TO BE UPDATED MANUALLY 
		uint8_t key = Barriers_Key;

		switch (key) {
		case NO_BARRIERS:
			return "NO_BARRIERS";
		case VERTICAL_BARRIER:
			return "VERTICAL_BARRIER";
		case HORIZONTAL_BARRIER:
			return "HORIZONTAL_BARRIER";

		default:
			return "n/a";
		}

	}
	std::string_view interactives_printable() { // HAS TO BE UPDATED MANUALLY 
		uint8_t key = Interactives_Key;

		switch (key) {
		case NO_INTERACTIVES:
			return "NO_INTERACTIVES";
		case SPARSE_FOOD:
			return "SPARSE_FOOD";
		case DENSE_FOOD:
			return "DENSE_FOOD";
		case FOUR_FLAGS:
			return "FOUR_FLAGS";

		default:
			return "n/a";
		}

	}
	std::string_view neurons_printable(uint8_t neuron) { // HAS TO BE UPDATED MANUALLY 
		uint8_t key = neuron;
		
		switch (key) {
		case sX_POS:
			return "sX_POS";
		case sY_POS:
			return "sY_POS";
		case sPOP_DENSITY_FWD:
			return "sPOP_DENSITY_FWD";
		case sAGE:
			return "sAGE";
		case iINT_1:
			return "iINT_1";
		case iINT_2:
			return "iINT_2";
		case iINT_3:
			return "iINT_3";
		case aMOVE_W:
			return "aMOVE_W";
		case aMOVE_E:
			return "aMOVE_E";
		case aMOVE_N:
			return "aMOVE_N";
		case aMOVE_S:
			return "aMOVE_S";

		default:
			return "n/a";
		}

	}



	// metagenomes / startingpopulations
	void saveMetagenome(World* w) {

	}

	const std::vector<startingGenome> readMetagenome() {

		std::vector<startingGenome> metagenome;

		std::string filepath = static_cast<std::string>(startingPop_path);
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "Error opening metagenome file!\n";
		}

		std::string line;

		std::array<std::array<uint32_t, numberOfGenes>, numberOfPixies> startingPopulation;

		// first line contains numberOfPixies, numberOfGenes
		std::getline(file, line);
		std::stringstream header(line);

		std::string numPixies_str;
		std::getline(header, numPixies_str, ',');
		int numPixies = std::stoi(numPixies_str);
		std::string numGenes_str;
		std::getline(header, numGenes_str, ',');
		int numGenes = std::stoi(numGenes_str);

		assert(numPixies == numberOfPixies);
		assert(numGenes == numberOfGenes);

		// following lines contain numClones,colR,colG,colB,gene1,gene2,gene3,gene3,...
		while (std::getline(file, line)) {
			std::stringstream line_stream(line);

			// 1. numClones
			std::string numClones_str;
			std::getline(line_stream, numClones_str, ',');
			int numClones = std::stoi(numClones_str);

			// 2. color channels
			std::string colR_str;
			std::getline(line_stream, colR_str, ',');
			uint8_t colR = std::stoi(colR_str);
			std::string colG_str;
			std::getline(line_stream, colG_str, ',');
			uint8_t colG = std::stoi(colG_str);
			std::string colB_str;
			std::getline(line_stream, colB_str, ',');
			uint8_t colB = std::stoi(colB_str);
			Color Col{ colR, colG, colB };

			// all genes
			int geneCount{};
			std::string dna_str;
			std::array<uint32_t, numberOfGenes> dna_array{};
			while (std::getline(line_stream, dna_str, ',')) {

				dna_array[geneCount] = std::stoi(dna_str, nullptr, 0);
				geneCount++;
			}

			// append to metagenome
			metagenome.push_back(startingGenome{ dna_array, Col, numClones });
		}

		return metagenome;
	}

	// GIFs
	bool shouldCreateGIF(int gen) { // check if the current generation lies within the gens-to-be-rendered for the corresponding rendering mode
		if (createGIF == "selected") {
			return std::find(createGIFfor.begin(), createGIFfor.end(), gen) != createGIFfor.end();
		}
		else if (createGIF == "every") {
			return gen % createGIFevery == 0;
		}
		else if (createGIF == "last") {
			return gen == numberOfGenerations;
		}
		else if (createGIF == "first&last") {
			return gen == 1 || gen == numberOfGenerations;
		}
		else { return false; }
	}
	void writeGIFdata(const int generation, const World* w) {
		/*
			Für jede Generation müsste eine Textdatei erstellt (und am besten in einem Ordner "GIF data" abgespeichert)
			werden. Jede Zeile entspricht einem SimStep und muss daher im "append"-modus hinzugefügt werden.
			Für jedes Pixie muss ein Vektor/Tupel in folgendem Format abgespeichert werden:

			(shape;yxPos[0];yxPos[1];facing;color[0];color[1];color[2]),

			also insgesamt 7 variablen (ints) pro pixie.
			Das appenden kann entweder so gelöst werden, dass eine funktion die leere datei für jede Generation erstellt
			und den Dateinamen in einer globalen Variable speichert; und dass dann eine andere Funktion (diese hier) auf
			diese Variable zugreift und der Datei eine Zeile anhängt.
			Oder es wird nur diese Funktion benutzt, immer das Argument "generation" benutzt um mit GIFdata_x auf die
			jeweilige Datei zuzugreifen und immer den append-modus zu benutzen. Hoffentlich funktioniert das einwandfrei


			Das Python-skript kann dann den Dateinamen GIFdata_x für alle möglichen x < 10.000 oder so ausprobieren
			(oder einfach für alle dateien im ordner: os.listdir / pathlib)


			THIS FUNCTION CAN BE FURTHER IMPROVED BY USING STD::TO_CHARS AND A SINGLE WRITE()-CALL TO BUFFER OUTPUT STRING.
		*/

		// open (new) folder
		std::ostringstream oss;
		oss << bs::folder_dir << "/GIF data";
		std::string folderPath = oss.str();

		std::filesystem::create_directory(folderPath);

		// open (new) file
		std::ostringstream oss2;
		oss2 << folderPath << "/GIFdata_" << generation << ".txt";
		std::string fileName = oss2.str();

		// Check if file exists and/or is empty
		std::ifstream infile(fileName, std::ios::binary | std::ios::ate);
		bool file_empty = true;
		if (infile) {
			file_empty = (infile.tellg() == 0);  // size in bytes
			infile.close();
		}

		// open file
		//std::ofstream file(fileName, std::ios::out | std::ios::app); //append //COULD ALSO OPEN IN BINARY IF ONLY WRITING BYTES
		std::ofstream file(fileName, std::ios::binary | std::ios::app); //append 
		if (!file.is_open()) {
			std::cout << "could not open csv file :'^(\n";
			return;
		}

		// write header
		if (file_empty) { 
			file << gridsizeY << ',' << gridsizeX << ',' << GIF_resolution << ',' << static_cast<int>(selectionCriterium) << '\n';

			// write one line of environment positions
			for (size_t i = 0; i < barriers.size(); i++) {
				Position& pos = barriers[i];
				if (i > 0) file << ',';
				file << pos.yPos << ';' << pos.xPos;
			}
			file << '\n';
		}

		// write state of every pixie
		const std::vector<Entity> inhabitants = w->fitness.get_entities();
		size_t inh_size = inhabitants.size();

		std::string out; 
		out.reserve(inh_size * 32 + interactives.size() * 12); // ~30 byte per pixie, 12 bites per environment

		char temp[32]; // for ints
		std::ostringstream floatBuffer; // for floats
		floatBuffer.setf(std::ios::fixed);
		floatBuffer.precision(3);

		for (size_t i = 0; i < inh_size; i++) {
			const Entity& p = inhabitants[i];
			// print the pixies Position (y, x), its facing direction and three Color channels
			const Position &cache_pos = w->Pos.get(p);
			const float facing = w->facing.get(p);
			const Entity gnm_index = w->PixieGenomes.get(p);
			const Color gnm = w->genome.get(gnm_index).col;

			//if (i > 0) file << ","; // pixies are separated by commata
			if (i > 0) out.push_back(',');
			/*file << cache_pos.yPos << ";" << cache_pos.xPos << ";"
				<< facing << ";"
				<< static_cast<int>(gnm.r) << ";"
				<< static_cast<int>(gnm.g) << ";"
				<< static_cast<int>(gnm.b);*/
			auto res = std::to_chars(temp, temp + sizeof(temp), cache_pos.yPos);
			out.append(temp, res.ptr);
			out.push_back(';');

			res = std::to_chars(temp, temp + sizeof(temp), cache_pos.xPos);
			out.append(temp, res.ptr);
			out.push_back(';');

			// facing 
			floatBuffer.str("");
			floatBuffer.clear();
			floatBuffer << facing;
			out.append(floatBuffer.str()); out.push_back(';');


			// colors (cast to int as before)
			res = std::to_chars(temp, temp + sizeof(temp), static_cast<int>(gnm.r));
			out.append(temp, res.ptr); out.push_back(';');

			res = std::to_chars(temp, temp + sizeof(temp), static_cast<int>(gnm.g));
			out.append(temp, res.ptr); out.push_back(';');

			res = std::to_chars(temp, temp + sizeof(temp), static_cast<int>(gnm.b));
			out.append(temp, res.ptr);
			
		}
		out.push_back('\n');
		
		// include something for the environment 
		for (int i = 0; i < interactives.size(); i++) {

			if (i > 0) out.push_back(',');

			Position& pos = interactives[i].Position;

			out += std::to_string(static_cast<int>(pos.yPos));
			out.push_back(';');
			out += std::to_string(static_cast<int>(pos.xPos));
			out.push_back(';');
			out += std::to_string(interactives[i].type);
		}
		out.push_back('\n');

		// write into file
		file.write(out.data(), static_cast<std::streamsize>(out.size()));
		//file << '\n';
		

		file.close();
	}
	void renderGIFs() {
		// open python script
		int ret = std::system("python render_GIFs.py");
	}

}