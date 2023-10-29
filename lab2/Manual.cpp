#include <fstream>
#include <sstream>
#include "Manual.h"

Manual::Converter::Converter() {
	name = "";
}

int Manual::getSize() const {
	return static_cast<int>(list.size());
}

std::vector<struct Manual::Converter> Manual::getList() {
	return list;
}

void Manual::readMan(const std::string& name) {
	std::ifstream man(name, std::ios::in | std::ios::binary);
	std::string commandStr;
    std::string nameOfCom;

	while (std::getline(man, commandStr)) {
        std::stringstream command(commandStr);

        command >> nameOfCom;

        int num = 0,
            count = 2;
        Converter newConverter;
        newConverter.name = nameOfCom;

        if (nameOfCom == "slowed" || nameOfCom == "reverb"){
            count = 3;
        }

        for (int i = 0; i < count; i++) {
            command >> num;
            newConverter.parameters.push_back(num);
        }

        list.push_back(newConverter);
	}
}