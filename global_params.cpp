#include "global_params.h"

#include <chrono>
#include <fstream>
#include <filesystem>
//#include <iomanip>
#include <sstream>

namespace bs {
	//dynamic variables are initialized here


	int generationAge{};

	// random engine
	Random* randomengine = new bs::Random();

	// folder directories
	std::string folder_dir = "";
	std::string cwd_str = "";

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
