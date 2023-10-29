#include <string>
#include <vector>

class Manual {
	struct Converter {
		std::string name;
		std::vector<int> parameters;

		Converter();
	};

	std::vector<struct Converter> list;

public:
	[[nodiscard]] int getSize() const;
	std::vector<struct Converter> getList();

	void readMan(const std::string& name);
};