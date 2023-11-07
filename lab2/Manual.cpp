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

        int num = 0;
        Converter newConverter;
        newConverter.name = nameOfCom;

        while (!command.eof()) {
            if (!(command >> num)) {
                throw ErrorType();
            }

            newConverter.parameters.push_back(num);
        }

        list.push_back(newConverter);
    }
}