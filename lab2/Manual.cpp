#include <fstream>
#include <memory>
#include "Manual.h"

template <char ch0, char ch1, char ch2, char ch3> struct MakeWord { enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) }; };

Manual::Converter::Converter() {
	name = "";
}

Manual::Manual() {
	listSize = 0;
}

int Manual::getSize(void) {
	return listSize;
}

std::vector<struct Manual::Converter> Manual::getVector(void) {
	return list;
}

void Manual::readMan(std::string name) {
	std::ifstream man(name, std::ios::in | std::ios::binary);
	uint32_t begin = 0;
	uint32_t oldBegin = 0;

	man.read(reinterpret_cast<char*>(&begin), sizeof(begin));

	while (begin != oldBegin) { //mb to do norm reading txt
		if (begin == MakeWord<'m', 'u', 't', 'e'>::value) {
			Converter newConverter;
			list.push_back(newConverter);
			readMute(listSize, man);
			listSize++;
		}

		if (begin == MakeWord<'m', 'i', 'x', ' '>::value) {
			Converter newConverter;
			list.push_back(newConverter);
			readMix(listSize, man);
			listSize++;
		}

		if (begin == MakeWord<'m', 'i', 'x', 'A'>::value) {
			Converter newConverter;
			list.push_back(newConverter);
			readMixAlt(listSize, man);
			listSize++;
		}

		if (begin == MakeWord<'s', 'l', 'o', 'w'>::value) {
			Converter newConverter;
			list.push_back(newConverter);
			readSlowed(listSize, man);
			listSize++;
		}

		if (begin == MakeWord<'r', 'e', 'v', 'e'>::value) {
			Converter newConverter;
			list.push_back(newConverter);
			readReverb(listSize, man);
			listSize++;
		}

		oldBegin = begin;
		man.read(reinterpret_cast<char*>(&begin), sizeof(begin));
	}
}

void Manual::readMute(int converterId, std::ifstream& man) {
	int8_t byte = 0;

	list[converterId].name = "mute";
	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[0] *= 10;
		list[converterId].parameters[0] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	int pos = 0;
	int oldPos = 0;

	while (byte != ' ' && byte != '\n' && oldPos != pos) {
		list[converterId].parameters[1] *= 10;
		list[converterId].parameters[1] += atoi(reinterpret_cast<char*>(&byte));
		oldPos = static_cast<int>(man.tellg());
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
		pos = static_cast<int>(man.tellg());
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}
}

void Manual::readMix(int converterId, std::ifstream& man) {
	int8_t byte = 0;

	list[converterId].name = "mix";
	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[0] *= 10;
		list[converterId].parameters[0] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	int pos = 0;
	int oldPos = 0;

	while (byte != ' ' && byte != '\n' && oldPos != pos) {
		list[converterId].parameters[1] *= 10;
		list[converterId].parameters[1] += atoi(reinterpret_cast<char*>(&byte));
		oldPos = static_cast<int>(man.tellg());
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
		pos = static_cast<int>(man.tellg());
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}
}

void Manual::readMixAlt(int converterId, std::ifstream& man) {
	int8_t byte = 0;

	list[converterId].name = "mixAlt";
	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	while (byte != ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[0] *= 10;
		list[converterId].parameters[0] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	int pos = 0;
	int oldPos = 0;

	while (byte != ' ' && byte != '\n' && oldPos != pos) {
		list[converterId].parameters[1] *= 10;
		list[converterId].parameters[1] += atoi(reinterpret_cast<char*>(&byte));
		oldPos = static_cast<int>(man.tellg());
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
		pos = static_cast<int>(man.tellg());
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}
}

void Manual::readSlowed(int converterId, std::ifstream& man) {
	int8_t byte = 0;

	list[converterId].name = "slowed";
	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	while (byte != ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[0] *= 10;
		list[converterId].parameters[0] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[1] *= 10;
		list[converterId].parameters[1] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	int pos = 0;
	int oldPos = 0;

	while (byte != ' ' && byte != '\n' && oldPos != pos) {
		list[converterId].parameters[2] *= 10;
		list[converterId].parameters[2] += atoi(reinterpret_cast<char*>(&byte));
		oldPos = static_cast<int>(man.tellg());
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
		pos = static_cast<int>(man.tellg());
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}
}

void Manual::readReverb(int converterId, std::ifstream& man) {
	int8_t byte = 0;

	list[converterId].name = "reverb";
	man.read(reinterpret_cast<char*>(&byte), sizeof(byte));

	while (byte != ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[0] *= 10;
		list[converterId].parameters[0] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	while (byte != ' ') {
		list[converterId].parameters[1] *= 10;
		list[converterId].parameters[1] += atoi(reinterpret_cast<char*>(&byte));
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}

	list[converterId].parameters.push_back(0);

	int pos = 0;
	int oldPos = 0;

	while (byte != ' ' && byte != '\n' && oldPos != pos) {
		list[converterId].parameters[2] *= 10;
		list[converterId].parameters[2] += atoi(reinterpret_cast<char*>(&byte));
		oldPos = static_cast<int>(man.tellg());
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
		pos = static_cast<int>(man.tellg());
	}

	while (byte == ' ') {
		man.read(reinterpret_cast<char*>(&byte), sizeof(byte));
	}
}
