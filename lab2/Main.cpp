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

	auto tracklist = new Track[argc - 2];

	for (int i = 1; i < argc - 1; i++) {
		std::string soundName = argv[i];
		tracklist[i - 1].readHead(soundName);
	}

	tracklist[0].writeHead("result.wav");

	std::string manualName = argv[argc - 1];
	auto man = new Manual;
	man->readMan(manualName);

	int listSize = man->getSize();
	
	for (int i = 0; i < listSize; i++) {
        Converter* converter;
        std::string name = man->getList()[i].name;
        std::vector<int> parameters = man->getList()[i].parameters;

		if (name == "mute") {
            CallMute mute;
            converter = mute.factoryMethod();
		}

		if (name == "mix") {
            CallMix mix;
            converter = mix.factoryMethod();
        }

		if (name == "mixAlt") {
            CallMixAlt mixAlt;
            converter = mixAlt.factoryMethod();
        }

		if (name == "slowed") {
            CallSlowed slowed;
            converter = slowed.factoryMethod();
        }

		if (name == "reverb") {
            CallReverb reverb;
            converter = reverb.factoryMethod();
        }

        converter->convert(tracklist, parameters);
	}

	delete man;
	delete[] tracklist;

	return 0;
}