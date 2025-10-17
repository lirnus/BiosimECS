#include "analysis.h"

#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <charconv>

namespace bs {


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
			file << gridsizeY << "," << gridsizeX << "," << GIF_resolution << "," << static_cast<int>(selectionCriterium) << "\n";
		}

		// write state of every pixie
		const std::vector<Entity> inhabitants = w->fitness.get_entities();
		size_t inh_size = inhabitants.size();

		std::string out; 
		out.reserve(inh_size * 32); // ~30 byte per pixie

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
		file.write(out.data(), static_cast<std::streamsize>(out.size()));
		//file << '\n';

		// include something for the environment 
		// for example, create a new line that only contains the positions of each BARRIER entity on the grid,
		// and a new line for the positions of each FOOD. if there are none, the lines stay empty
		

		file.close();
	}
	void renderGIFs() {
		// open python script
		int ret = std::system("python render_GIFs.py");
	}

}