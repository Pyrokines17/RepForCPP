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

	std::vector<Track> tracklist;
	std::string soundName = "";

	for (int i = 1; i < argc - 1; i++) {
		soundName = argv[i];
		Track newTrack;
		newTrack.readHead(soundName);
		tracklist.push_back(newTrack);
	}

	soundName = "result.wav";
	tracklist[0].writeHead(soundName);
	tracklist[0].copyData();

	Track resTrack;
	resTrack.readHead(soundName);

	std::string manualName = argv[argc - 1];
	Manual man;
	man.readMan(manualName);

	int listSize = man.getSize();

	std::vector<std::shared_ptr<std::ifstream>> inStreams;
	std::vector<std::shared_ptr<std::fstream>> outStreams;

	inStreams.push_back(resTrack.getInStr());
	outStreams.push_back(tracklist[0].getOutStr());
	
	for (int i = 0; i < listSize; i++) {
        Converter* converter = nullptr;
        std::string name = man.getList()[i].name;
        std::vector<int> parameters = man.getList()[i].parameters;

		if (name == "mute") {
            CallMute mute;
            converter = mute.factoryMethod();
		}

		if (name == "mix") {
            CallMix mix;
			int idOfTrack = parameters[0] - 1;
			parameters.push_back(tracklist[0].getFinish());
			inStreams.push_back(tracklist[idOfTrack].getInStr());
            converter = mix.factoryMethod();
        }

		if (name == "mixAlt") {
            CallMixAlt mixAlt;
			int idOfTrack = parameters[0] - 1;
			inStreams.push_back(tracklist[0].getInStr()); //origTrack
			parameters.push_back(tracklist[0].getFinish());
			inStreams.push_back(tracklist[idOfTrack].getInStr());
            converter = mixAlt.factoryMethod();
        }

		if (name == "slowed") {
            CallSlowed slowed;
			inStreams.push_back(tracklist[0].getInStr()); //origTrack
			parameters.push_back(tracklist[0].getFinish());
            converter = slowed.factoryMethod();
        }

		if (name == "reverb") {
            CallReverb reverb;
            converter = reverb.factoryMethod();
        }

        converter->convert(inStreams, outStreams, parameters);

		while (inStreams.size() > 1) {
			inStreams.erase(inStreams.end() - 1);
		}
	}

	return 0;
}