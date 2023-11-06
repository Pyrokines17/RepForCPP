#include <fstream>
#include <sstream>
#include <iostream>
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
	std::ifstream man;
    std::string commandStr;
    std::string nameOfCom;

    man.open(name, std::ios::in | std::ios::binary);

    if (!man.is_open()) {
        throw CanNotOpen();
    }

    while (std::getline(man, commandStr)) {
        if (man.bad()) {
            throw BadError();
        }
        if (man.fail()) {
            throw CritError();
        }

        std::stringstream command(commandStr);
        command >> nameOfCom;

        if (nameOfCom != "mute" && nameOfCom != "mix" && nameOfCom != "mixAlt" && nameOfCom != "slowed" && nameOfCom != "reverb") {
            throw ComNotFound();
        }

        int num = 0,
            count = 2;
        Converter newConverter;
        newConverter.name = nameOfCom;

        if (nameOfCom == "slowed" || nameOfCom == "reverb") {
            count = 3;
        }

        for (int i = 0; i < count; i++) {
            if (command.eof()) {
                throw IncorNumOfPar();
            }

            if (!(command >> num)) {
                throw ErrorType();
            }

            newConverter.parameters.push_back(num);
        }

        if (nameOfCom == "mute" || nameOfCom == "slowed" || nameOfCom == "reverb") {
            if (newConverter.parameters[1] < newConverter.parameters[0]) {
                throw IncorBordOfPar();
            }
        }

        list.push_back(newConverter);
    }
}