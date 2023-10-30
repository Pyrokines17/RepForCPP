#include "Converters.h"

void Mute::convert(std::vector<std::shared_ptr<std::ifstream>> inStreams, std::vector<std::shared_ptr<std::fstream>> outStreams, const std::vector<int>& parameters) {
    std::shared_ptr<std::fstream> outStream = outStreams[0];
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());

    int16_t sample = 0;

    uint32_t beginBorder = comSampleRate * static_cast<uint32_t>(parameters[0]);
    uint32_t endBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);

    outStream->seekp(startWriteIndicator + beginBorder * 2);
    uint32_t position = beginBorder;

    while (position < endBorder) {
        outStream->write(reinterpret_cast<const char*>(&sample), sizeof(int16_t));
        position++;
    }

    outStream->seekp(startWriteIndicator);
}

void Mix::convert(std::vector<std::shared_ptr<std::ifstream>> inStreams, std::vector<std::shared_ptr<std::fstream>> outStreams, const std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = inStreams[0];
    uint32_t startReadIndicator1 = static_cast<uint32_t>(inStream->tellg());
    std::shared_ptr<std::fstream> outStream = outStreams[0];
    uint32_t startWriteIndicator1 = static_cast<uint32_t>(outStream->tellp());
    std::shared_ptr<std::ifstream> secInStream = inStreams[1];
    uint32_t startReadIndicator2 = static_cast<uint32_t>(secInStream->tellg());

    int16_t sampleOfFirst = 0;
    int16_t sampleOfSecond = 0;
    int16_t result = 0;

    uint32_t startBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);
    uint32_t finish = parameters[2] / 2;

    inStream->seekg(startReadIndicator1 + startBorder * 2);
    outStream->seekp(startWriteIndicator1 + startBorder * 2);
    uint32_t position = startBorder;

    while (position < finish) {
        inStream->read(reinterpret_cast<char*>(&sampleOfFirst), sizeof(int16_t));
        secInStream->read(reinterpret_cast<char*>(&sampleOfSecond), sizeof(int16_t));

        result = (sampleOfFirst + sampleOfSecond) / 2;

        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int16_t));
        position++;
    }

    inStream->seekg(startReadIndicator1);
    outStream->seekp(startWriteIndicator1);
    secInStream->seekg(startReadIndicator2);
}

void MixAlt::convert(std::vector<std::shared_ptr<std::ifstream>> inStreams, std::vector<std::shared_ptr<std::fstream>> outStreams, const std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = inStreams[2];
    uint32_t startReadIndicator1 = static_cast<uint32_t>(inStream->tellg());
    std::shared_ptr<std::fstream> outStream = outStreams[0];
    uint32_t startWriteIndicator1 = static_cast<uint32_t>(outStream->tellp());
    std::shared_ptr<std::ifstream> secInStream = inStreams[1];
    uint32_t startReadIndicator2 = static_cast<uint32_t>(secInStream->tellg());

    int16_t sample = 0;

    uint32_t borderOfSec = comSampleRate;
    uint32_t startBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);
    uint32_t finish = parameters[2] / 2;

    inStream->seekg(startReadIndicator1 + startBorder * 2);
    outStream->seekp(startWriteIndicator1 + startBorder * 2);
    uint32_t position = startBorder;

    while (position < finish) {
        for (uint32_t i = 0; i < borderOfSec; i++) {
            inStream->read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            outStream->write(reinterpret_cast<const char*>(&sample), sizeof(int16_t));
            position++;
        }

        for (uint32_t i = 0; i < borderOfSec; i++) {
            secInStream->read(reinterpret_cast<char*>(&sample), sizeof(int16_t));
            outStream->write(reinterpret_cast<const char*>(&sample), sizeof(int16_t));
            position++;
        }
    }

    inStream->seekg(startReadIndicator1);
    outStream->seekp(startWriteIndicator1);
    secInStream->seekg(startReadIndicator2);
}

void Slowed::convert(std::vector<std::shared_ptr<std::ifstream>> inStreams, std::vector<std::shared_ptr<std::fstream>> outStreams, const std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = inStreams[1];
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    std::shared_ptr<std::fstream> outStream = outStreams[0];
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());

    int16_t firstSample = 0;
    int16_t secondSample = 0;
    int16_t result = 0;

    uint32_t beginBorder = comSampleRate * static_cast<uint32_t>(parameters[0]);
    uint32_t endBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);

    uint32_t finish = parameters[3] / 2;

    inStream->seekg(startReadIndicator + beginBorder * 2);
    outStream->seekp(startWriteIndicator + beginBorder * 2);
    uint32_t position = beginBorder;

    inStream->read(reinterpret_cast<char*>(&firstSample), sizeof(int16_t));
    position++;

    while (position < endBorder) {
        inStream->read(reinterpret_cast<char*>(&secondSample), sizeof(int16_t));
        result = (firstSample + secondSample) / static_cast<int16_t>(parameters[2]);

        outStream->write(reinterpret_cast<const char*>(&firstSample), sizeof(int16_t));
        
        for (int i = 1; i < parameters[2]; i++) {
            int16_t iResult = result * static_cast<int16_t>(i);
            outStream->write(reinterpret_cast<const char*>(&iResult), sizeof(int16_t));
        }

        firstSample = secondSample;
        position++;
    }

    outStream->write(reinterpret_cast<const char*>(&firstSample), sizeof(int16_t));

    while (position < finish) {
        inStream->read(reinterpret_cast<char*>(&firstSample), sizeof(int16_t));
        outStream->write(reinterpret_cast<const char*>(&firstSample), sizeof(int16_t));
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}

void Reverb::convert(std::vector<std::shared_ptr<std::ifstream>> inStreams, std::vector<std::shared_ptr<std::fstream>> outStreams, const std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = inStreams[0];
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    std::shared_ptr<std::fstream> outStream = outStreams[0];
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());

    int16_t firstSample = 0;
    int16_t secondSample = 0;
    int16_t result = 0;

    uint32_t beginBorder = comSampleRate * static_cast<uint32_t>(parameters[0]);
    uint32_t endBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);

    uint32_t delay = delayMilliseconds * comSampleRate / 1000;
    float decay = 1.0f / static_cast<float>(parameters[2]);

    outStream->seekp(startWriteIndicator + (beginBorder + delay) * 2);
    uint32_t position = beginBorder;

    while (position < endBorder - delay) {
        inStream->seekg(startReadIndicator + (position + delay) * 2);
        inStream->read(reinterpret_cast<char*>(&firstSample), sizeof(int16_t));
        inStream->seekg(startReadIndicator + position * 2);
        inStream->read(reinterpret_cast<char*>(&secondSample), sizeof(int16_t));

        result = firstSample + static_cast<int16_t>(static_cast<float>(secondSample) * decay);
        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int16_t));
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}
