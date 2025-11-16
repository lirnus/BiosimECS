#include "analysis.h"

#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <charconv>
#include <iomanip>

namespace bs {

	// metadata
	void saveMetaData(const std::string& configFile) {
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
		file << "### Pixies Evolutionary Sandbox Simulation ###\n\n";

		file << "# experiment from " << std::put_time(&tm, "%Y/%m/%d %H:%M") << "\n";
		file << "# file directory: " << folder_dir << "\n";
		file << "# Time elapsed during simulation: " << simulationTime << " sec";
		file << "; Time elapsed during rendering: " << renderingTime << " sec\n";
		
		/*
		file << "\n-World parameters-\n";
		file << "gridsizeX: "			<< worldParams->gridSizeX				<< "\n";
		file << "gridsizeY: "			<< worldParams->gridSizeY				<< "\n";
		file << "numberOfPixies: "		<< worldParams->numberOfPixies			<< "\n";
		file << "numberOfGenes: "		<< worldParams->numberOfGenes			<< "\n";
		file << "numberOfGenerations: "	<< worldParams->numberOfGenerations		<< "\n";
		file << "numberOfSimSteps: "	<< worldParams->numberOfSimSteps			<< "\n";
		file << "selectionCriterium: "	<< selCrit_printable()		<< "\n";
		file << "Barriers_Key: "		<< barriers_printable()		<< "\n";
		file << "Interactives_Key: "	<< interactives_printable() << "\n";

		file << "\n-Simulator settings-\n";
		file << "startingPopulation: "	<< (simParams->startingPopulation ? "true" : "false") << "\n";
		file << "startingPop_path: "	<< simParams->startingPop_path			<< "\n";

		file << "\n-Continuous simulator settings-\n";
		// n/a

		file << "\n-Population settings-\n";
		file << "blockedByOtherPixies: " << (popParams->blockedByOtherPixies ? "true" : "false") << "\n";
		file << "pixies_per_genome: "	<< popParams->pixies_per_genome		<< "\n";

		file << "\n-Pixie parameters-\n";
		file << "mutationRate: "		<< pixParams->mutationRate				<< "\n";
		file << "weight_factor: "		<< pixParams->weight_factor			<< "\n";
		file << "defaultSearchRadius: "	<< pixParams->defaultSearchRadius		<< "\n";

		file << "\n-Reproducibility-\n";
		file << "deterministic: " << (rngParams->deterministic ? "true" : "false") << "\n";
		file << "random seed: " << rngParams->seeed << "\n";

		file << "\n-Analysis-\n";
		file << "calc_pop_stats: " << (analParams->calc_pop_stats ? "true" : "false") << "\n";
		file << "save_metagenome: " << analParams->save_metagenome << " -- save the metagenome: 'none', 'last', 'first&last', 'every' or 'selected'" << "\n";
		file << "saveMetagenomeEvery: " << analParams->saveMetagenomeEvery << "\n";
		file << "saveMetagenomeFor: {"; 
		for (int i : analParams->saveMetagenomeFor) { file << " " << i << ","; }
		file << " }\n";

		file << "\n-Render Settings-\n";
		file << "createGIF: " << renderParams->createGIF << " -- render mode: 'none', 'last', 'first&last', 'every' or 'selected'" << "\n";
		file << "GIF_resolution: " << renderParams->GIF_resolution << "\n";
		file << "createGIFevery: " << renderParams->createGIFevery << "\n";
		file << "createGIFfor: {";
		for (int i : renderParams->createGIFfor) { file << " " << i << ","; }
		file << " }\n";
		file << "color_variation: " << renderParams->color_variation << "\n";

		file << "\nEnabled Neurons:\n";
		for (int i = 0; i < MAX_NEURONS; i++) {
			file << neurons_printable(i) << "\n";
		}
		*/

		// append simconfig.ini 
		std::ifstream simconfig_file(configFile);

		std::string line;
		while (std::getline(simconfig_file, line)) {
			file << line << "\n";
		}

		// close
		file.close();
	}
	std::string_view selCrit_printable() { // HAS TO BE UPDATED MANUALLY 
		uint8_t key = worldParams->selectionCriterium;

		switch (key) {
		case NO_SELECTION:
			return "NO_SELECTION";
		case KILLRIGHTHALF:
			return "KILLRIGHTHALF";
		case GOLEFT_GRADIENT:
			return "GOLEFT_GRADIENT";
		default:
			return "n/a";
		}
		
	}
	std::string_view barriers_printable() { // HAS TO BE UPDATED MANUALLY 
		uint8_t key = worldParams->Barriers_Key;

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
		uint8_t key = worldParams->Interactives_Key;

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
		
		/*switch (key) {
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
		}*/
		return "under maintenance";
	}


	// metagenomes / startingpopulations
	void saveMetagenome(World* w, int gen) {
		// open (new) folder
		std::ostringstream folder;
		folder << bs::folder_dir << "/Metagenomes";
		std::string folderPath = folder.str();

		std::filesystem::create_directory(folderPath);

		// calculate number of digits for numberOfGenerations
		int digits = floor(log10(worldParams->numberOfGenerations)) + 1;
		std::ostringstream generation;
		generation << std::setw(digits) << std::setfill('0') << gen;

		std::ostringstream oss;
		oss << folderPath << "/metagenome-" << generation.str() << ".csv";
		std::string fileName = oss.str();

		// open
		std::ofstream file(fileName);
		if (!file.is_open()) {
			std::cout << "could not open metagenome file :(\n";
			return;
		}

		// write header
		file << worldParams->numberOfPixies << ',' << worldParams->numberOfGenes << '\n';

		// write genomes
		
		// for each genome entity, count how many pixie_genomes have that entity = numClones
		std::vector<Entity> genome_entities = w->genome.get_entities(); // the actual brain templates
		for (Entity& g_e : genome_entities) {
			
			int numClones{};
			w->PixieGenomes.for_each([&](Entity e, Entity c) { if (c == g_e) numClones++; });
			
			const Genome& gnm = w->genome.get(g_e);

			file << numClones << ',';
			file << static_cast<int>(gnm.col.r) << ','
				<< static_cast<int>(gnm.col.g) << ','
				<< static_cast<int>(gnm.col.b);
			for (uint32_t gene : gnm.DNA) { 
				file << ",0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << gene << std::dec; // hexcode
			} 
			/*for (uint32_t gene : gnm.DNA) { file << "," <<  gene; }*/ // decimal

			file << '\n';
		}

		// close
		file.close();
	}
	bool shouldSaveMetagenome(int gen) { // check if the current generation lies within the gens-to-be-saved for the corresponding save_metagenome mode
		// the first and last generation are always included
		int numGens = worldParams->numberOfGenerations;
		std::string& sm = analParams->save_metagenome;
		int smEvery = analParams->saveMetagenomeEvery;
		std::vector<int>& smFor = analParams->saveMetagenomeFor;
		if (sm == "selected") {
			return gen == 1 || std::find(smFor.begin(), smFor.end(), gen) != smFor.end() || gen == numGens;
		}
		else if (sm == "every") {
			return gen == 1 || gen % smEvery == 0 || gen == numGens;
		}
		else if (sm == "first&last") {
			return gen == 1 || gen == numGens;
		}
		else { return false; }
	}
	const std::vector<startingGenome> readMetagenome() {

		std::vector<startingGenome> metagenome;

		std::string filepath = static_cast<std::string>(simParams->startingPop_path);
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "Error opening metagenome file!\n";
		}

		std::string line;


		// first line contains numberOfPixies, numberOfGenes
		std::getline(file, line);
		std::stringstream header(line);

		std::string numPixies_str;
		std::getline(header, numPixies_str, ',');
		int numPixies = std::stoi(numPixies_str);
		std::string numGenes_str;
		std::getline(header, numGenes_str, ',');
		const int numGenes = std::stoi(numGenes_str);

		assert(numPixies == worldParams->numberOfPixies);
		assert(numGenes == worldParams->numberOfGenes);

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
			//std::array<uint32_t, MAX_ADJ> dna_array{};
			std::vector<uint32_t> dna_array{};
			dna_array.reserve(numGenes);
			while (std::getline(line_stream, dna_str, ',')) {

				dna_array[geneCount] = std::stol(dna_str, nullptr, 0);
				geneCount++;
			}

			// append to metagenome
			metagenome.push_back(startingGenome{ dna_array, Col, numClones });
		}

		return metagenome;
	}

	// GIFs
	bool shouldCreateGIF(int gen) { // check if the current generation lies within the gens-to-be-rendered for the corresponding rendering mode
		int numGens = worldParams->numberOfGenerations;
		std::string& cg = renderParams->createGIF;
		int cgEvery = renderParams->createGIFevery;
		std::vector<int>& cgFor = renderParams->createGIFfor;
		if (cg == "selected") {
			return gen == 1 || std::find(cgFor.begin(), cgFor.end(), gen) != cgFor.end() || gen == numGens;
		}
		else if (cg == "every") {
			return gen == 1 || gen % cgEvery == 0 || gen == numGens;
		}
		else if (cg == "first&last") {
			return gen == 1 || gen == numGens;
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
			file << worldParams->gridSizeY << ',' 
				<< worldParams->gridSizeX << ',' 
				<< renderParams->GIF_resolution << ',' 
				<< renderParams->GIF_frameduration << ','
				<< static_cast<int>(worldParams->selectionCriterium) << '\n';

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

	// population stats
	void writeSurvivalRates(World* w, int gen) {
		// not the amount of pixies that reproduce, but the amount of pixies that have fitnes > 0 (and thus CAN reporoduce)
		
		// calculate survivalrate
		size_t survivor_count{};
		w->fitness.for_each([&](Entity p, float c) {if (c > 0) survivor_count++; });

		float survivalRate = static_cast<float>(survivor_count) / worldParams->numberOfPixies;

		// open textfile
		std::ostringstream oss;
		oss << bs::folder_dir << "/survivalrate.txt";
		std::string fileName = oss.str();

		std::ofstream file(fileName, std::ios::out | std::ios::app);
		if (!file.is_open()) {
			std::cout << "could not open survivalrate file :(\n";
			return;
		}

		// write
		file << gen << ',' << survivalRate << '\n';

		// close
		file.close();
	}
	void writeDiversityIndex(World* w, int gen) {
		// the amount of unique genomes compared to the entire population size

		// calculate diversity
		size_t genome_count = w->genome.size();

		float diversity = static_cast<float>(genome_count) / worldParams->numberOfPixies;

		// open textfile
		std::ostringstream oss;
		oss << bs::folder_dir << "/diversity.txt";
		std::string fileName = oss.str();

		std::ofstream file(fileName, std::ios::out | std::ios::app);
		if (!file.is_open()) {
			std::cout << "could not open diversity file :(\n";
			return;
		}

		// write
		file << gen << ',' << diversity << '\n';

		// close
		file.close();
	}
	void writeMeanFitness(World* w, int gen) {
		// the mean fitness value across the entire population (at the end of a generation)

		// calculate mean Fitness
		float summed_fitness{};
		w->fitness.for_each([&](Entity p, float c) { summed_fitness += c; });

		float mean_fitness = summed_fitness / worldParams->numberOfPixies;

		// open textfile
		std::ostringstream oss;
		oss << bs::folder_dir << "/mean_fitness.txt";
		std::string fileName = oss.str();

		std::ofstream file(fileName, std::ios::out | std::ios::app);
		if (!file.is_open()) {
			std::cout << "could not open mean_fitness file :(\n";
			return;
		}

		// write
		file << gen << ',' << mean_fitness << '\n';

		// close
		file.close();
	}


	// wrapper functions
	void writePopulationStats(World* w, int gen) {// write stats to textfile
		
		writeSurvivalRates(w, gen);
		writeDiversityIndex(w, gen);
		writeMeanFitness(w, gen);
		
	}
	void popGenAnalysis() {// call python scripts
		
		int ret = std::system("python popGen_analysis.py");
	}
}