#include <string>
#include <vector>

class Manual {
	int listSize;

	struct Converter {
		std::string name;
		std::vector<int> parameters;

		Converter();
	};

	std::vector<struct Converter> list;

public:
	Manual();

	int getSize(void);
	std::vector<struct Converter> getVector(void);

	void readMan(std::string name);

	void readMute(int converterId, std::ifstream& man);
	void readMix(int converterId, std::ifstream& man);
	void readMixAlt(int converterId, std::ifstream& man);
	void readSlowed(int converterId, std::ifstream& man);
	void readReverb(int converterId, std::ifstream& man);
};