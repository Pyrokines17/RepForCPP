#include "SoundProcessor.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "arg not found" << std::endl;
		return 1;
	}

	if (argc == 2) {
		std::string par = argv[1];

		if (par == "-h") {
			printHelp();
			return 0;
		}
		else {
			std::cout << "not enough arg" << std::endl;
			return 1;
		}
	}

	Track* trackList = new Track[argc - 2];

	for (int i = 1; i < argc - 1; i++) {
		std::string soundName = argv[i];
		trackList[i - 1].readHead(soundName);
	}

	trackList[0].writeHead("result.wav");

	std::string manualName = argv[argc - 1];
	Manual* man = new Manual;
	man->readMan(manualName);

	int listSize = man->getSize();
	std::vector<struct Manual::Converter> list = man->getVector();
	
	for (int i = 0; i < listSize; i++) { //mb to do fabric method
		if (list[i].name == "mute") {
			trackList[0].mute(list[i].parameters[0], list[i].parameters[1]);
		}

		if (list[i].name == "mix") {
			trackList[0].mix(trackList[list[i].parameters[0] - 1], list[i].parameters[1]);
		}

		if (list[i].name == "mixAlt") {
			trackList[0].mixAlt(trackList[list[i].parameters[0] - 1], list[i].parameters[1]);
		}

		if (list[i].name == "slowed") {
			trackList[0].slowed(list[i].parameters[0], list[i].parameters[1], list[i].parameters[2]);
		}

		if (list[i].name == "reverb") {
			trackList[0].slowed(list[i].parameters[0], list[i].parameters[1], list[i].parameters[2]);
		}
	}

	delete man;
	delete[] trackList;

	return 0; //prikol s resultatom? vozmogno nugno chitat iz resultata
}