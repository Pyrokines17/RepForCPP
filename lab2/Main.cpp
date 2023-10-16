#include <cstdio>
#include "SoundProcessor.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "arg not found" << std::endl;
		return 1;
	}

	soundProc track1;
	soundProc track2;
	soundProc track3;

	std::string soundName1 = argv[1];
	int time = atoi(argv[2]);
	std::string soundName2 = argv[3];
	std::string soundName3 = argv[4];

	if (track1.readHead(soundName1) == false) {
		return 1;
	}

	if (track2.readHead(soundName2) == false) {
		return 1;
	}

	if (track3.readHead(soundName3) == false) {
		return 1;
	}

	soundName1.insert(0, "1");

	track1.writeHead(soundName1);
	track1.mute(time);

	soundName2.insert(0, "1");

	track2.writeHead(soundName2);
	track2.mix(track3);

	soundName3.insert(0, "1");

	track3.writeHead(soundName3);
	track3.mixAlt(track2);

	return 0;
}