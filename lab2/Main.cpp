#include <cstdio>
#include "SoundProcessor.h"

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "arg not found" << std::endl;
		return 1;
	}

	soundProc track1;

	std::string soundName = argv[1];

	if (track1.readHead(soundName) == false) {
		return 1;
	}

	track1.writeHead();

	return 0;
}