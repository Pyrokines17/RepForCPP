#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

#include "ListOfExcep.h"

const uint32_t comSampleRate = 44100;
const int delayMilliseconds = 500;

void printHelp();

class Converter {
public:
	virtual void convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) = 0;
	virtual ~Converter() = default;
};

class Mute : public Converter {
public:
	void convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) override;
};

class Mix : public Converter {
public:
	void convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) override;
};

class MixAlt : public Converter {
public:
	void convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) override;
};

class Slowed : public Converter {
public:
	void convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) override;
};

class Reverb : public Converter { 
public:
	void convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) override;
};

std::unique_ptr<Converter> createConverter(const std::string& name);