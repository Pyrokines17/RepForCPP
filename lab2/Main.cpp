#include "SoundProcessor.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cerr << "arg not found" << std::endl;
		return 1;
	}

	if (argc == 2) {
		std::string par = argv[1];

		if (par == "-h") {
			printHelp();
			return 0;
		}
		else {
			std::cerr << "not enough arg" << std::endl;
			return 2;
		}
	}

	std::vector<Track> tracklist;
	std::string soundName;

	for (int i = 1; i < argc - 1; i++) {
		soundName = argv[i];
		Track newTrack;

        try {
            newTrack.readHead(soundName);
        } catch (const CanNotOpen& e) {
            std::cerr << e.what() << "(" << soundName << ")" << std::endl;
            return 3;
        } catch (const CanNotRead& e) {
            std::cerr << e.what() << "(" << soundName << ")" << std::endl;
            return 4;
        } catch (const RIFFNotFound& e) {
            std::cerr << e.what() << "(" << soundName << ")" << std::endl;
            return 8;
        }

		tracklist.push_back(newTrack);
	}

	soundName = "result.wav";

    try {
        tracklist[0].writeHead(soundName);
    } catch (const CanNotOpen& e) {
        std::cerr << e.what() << "(" << soundName << ")" << std::endl;
        return 3;
    } catch (const CanNotWrite& e) {
        std::cerr << e.what() << "(" << soundName << ")" << std::endl;
        return 5;
    }

    try {
        tracklist[0].copyData();
    } catch (const CanNotRead& e) {
        std::cerr << e.what() << "(from first file)" << std::endl;
        return 4;
    } catch (const CanNotWrite& e) {
        std::cerr << e.what() << "(to result file)" << std::endl;
        return 5;
    }

	Track resTrack;

    try {
        resTrack.readHead(soundName);
    } catch (const CanNotOpen& e) {
        std::cerr << e.what() << "(" << soundName << ")" << std::endl;
        return 3;
    } catch (const CanNotRead& e) {
        std::cerr << e.what() << "(" << soundName << ")" << std::endl;
        return 4;
    }

    Manual man;
	std::string manualName = argv[argc - 1];

    try {
        man.readMan(manualName);
    } catch (const CanNotOpen& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 3;
    } catch (const BadError& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 6;
    } catch (const CritError& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 7;
    } catch (const ComNotFound& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 9;
    } catch (const IncorNumOfPar& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 10;
    } catch (const IncorBordOfPar& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 11;
    } catch (const ErrorType& e) {
        std::cerr << e.what() << "(" << manualName << ")" << std::endl;
        return 12;
    }

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
			parameters.push_back(tracklist[0].getFinish());
			inStreams.push_back(tracklist[idOfTrack].getInStr());
            converter = mixAlt.factoryMethod();
        }

		if (name == "slowed") {
            CallSlowed slowed;
			inStreams.push_back(tracklist[0].getInStr());
			parameters.push_back(tracklist[0].getFinish());
            converter = slowed.factoryMethod();
        }

		if (name == "reverb") {
            CallReverb reverb;
            converter = reverb.factoryMethod();
        }

        try {
            converter->convert(inStreams, outStreams, parameters);
        } catch (const CanNotRead& e) {
            std::cerr << e.what() << "(in " << name << " )" << std::endl;
            return 4;
        } catch (const CanNotWrite& e) {
            std::cerr << e.what() << "(in " << name << " )" << std::endl;
            return 5;
        }

		if (inStreams.size() > 1) {
			inStreams.erase(inStreams.end() - 1);
		}
	}

	return 0;
}