#include "Converters.h"

void Mute::convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) {
    const std::shared_ptr<std::ifstream>& inStream = inStreams[0];
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    const std::shared_ptr<std::fstream>& outStream = outStreams[0];
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());

    int16_t sample = 0;

    uint32_t beginBorder = comSampleRate * static_cast<uint32_t>(parameters[0]);
    uint32_t endBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);

    inStream->seekg(startReadIndicator + beginBorder * 2);
    outStream->seekp(startWriteIndicator + beginBorder * 2);
    uint32_t position = beginBorder;

    while (position < endBorder) {
        if (!outStream->write(reinterpret_cast<const char*>(&sample), sizeof(int16_t))) {
            throw CanNotWrite();
        }
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}

void Mix::convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) {
    const std::shared_ptr<std::ifstream>& inStream = inStreams[0];
    uint32_t startReadIndicator1 = static_cast<uint32_t>(inStream->tellg());
    const std::shared_ptr<std::fstream>& outStream = outStreams[0];
    uint32_t startWriteIndicator1 = static_cast<uint32_t>(outStream->tellp());
    const std::shared_ptr<std::ifstream>& secInStream = inStreams[1];
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
        if (!inStream->read(reinterpret_cast<char*>(&sampleOfFirst), sizeof(int16_t))) {
            throw CanNotRead();
        }
        if (!secInStream->read(reinterpret_cast<char*>(&sampleOfSecond), sizeof(int16_t))) {
            throw CanNotWrite();
        }

        result = static_cast<int16_t>((sampleOfFirst + sampleOfSecond) / 2);

        if (!outStream->write(reinterpret_cast<const char*>(&result), sizeof(int16_t))) {
            throw CanNotWrite();
        }
        position++;
    }

    inStream->seekg(startReadIndicator1);
    outStream->seekp(startWriteIndicator1);
    secInStream->seekg(startReadIndicator2);
}

void MixAlt::convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) {
    const std::shared_ptr<std::fstream>& outStream = outStreams[0];
    uint32_t startWriteIndicator1 = static_cast<uint32_t>(outStream->tellp());
    const std::shared_ptr<std::ifstream>& secInStream = inStreams[1];
    uint32_t startReadIndicator2 = static_cast<uint32_t>(secInStream->tellg());

    int16_t sample = 0;

    uint32_t startBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);
    uint32_t finish = parameters[2] / 2;

    outStream->seekp(startWriteIndicator1 + startBorder * 2);
    uint32_t position = startBorder;

    while (position < finish) {
        for (uint32_t i = 0; i < comSampleRate; i++) {
            if (!secInStream->read(reinterpret_cast<char*>(&sample), sizeof(int16_t))) {
                throw CanNotRead();
            }
            if (!outStream->write(reinterpret_cast<const char*>(&sample), sizeof(int16_t))) {
                throw CanNotWrite();
            }
            position++;
        }

        uint32_t writeIndicator1 = static_cast<uint32_t>(outStream->tellp());
        outStream->seekp(writeIndicator1 + comSampleRate * 2);
        position += comSampleRate;
    }

    outStream->seekp(startWriteIndicator1);
    secInStream->seekg(startReadIndicator2);
}

void Slowed::convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) {
    const std::shared_ptr<std::ifstream>& inStream = inStreams[1];
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    const std::shared_ptr<std::fstream>& outStream = outStreams[0];
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());

    int16_t firstSample = 0;
    int16_t secondSample = 0;
    int16_t result;

    uint32_t beginBorder = comSampleRate * static_cast<uint32_t>(parameters[0]);
    uint32_t endBorder = comSampleRate * static_cast<uint32_t>(parameters[1]);
    uint32_t finish = parameters[3] / 2;

    inStream->seekg(startReadIndicator + beginBorder * 2);
    outStream->seekp(startWriteIndicator + beginBorder * 2);
    uint32_t position = beginBorder;

    if (!inStream->read(reinterpret_cast<char*>(&firstSample), sizeof(int16_t))) {
        throw CanNotRead();
    }

    while (position < endBorder) {
        if (!inStream->read(reinterpret_cast<char*>(&secondSample), sizeof(int16_t))) {
            throw CanNotRead();
        }
        result = static_cast<int16_t>((firstSample + secondSample) / parameters[2]);

        if (!outStream->write(reinterpret_cast<const char*>(&firstSample), sizeof(int16_t))) {
            throw CanNotWrite();
        }
        
        for (int i = 1; i < parameters[2]; i++) {
            auto iResult = static_cast<int16_t>(result * static_cast<int16_t>(i));
            if (!outStream->write(reinterpret_cast<const char*>(&iResult), sizeof(int16_t))) {
                throw CanNotWrite();
            }
        }

        firstSample = secondSample;
        position++;
    }

    outStream->write(reinterpret_cast<const char*>(&firstSample), sizeof(int16_t));

    while (position < finish - 2) {
        if (!inStream->read(reinterpret_cast<char*>(&firstSample), sizeof(int16_t))) {
            throw CanNotRead();
        }
        if (!outStream->write(reinterpret_cast<const char*>(&firstSample), sizeof(int16_t))) {
            throw CanNotWrite();
        }
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}

void Reverb::convert(const std::vector<std::shared_ptr<std::ifstream>>& inStreams, const std::vector<std::shared_ptr<std::fstream>>& outStreams, const std::vector<int>& parameters) {
    const std::shared_ptr<std::ifstream>& inStream = inStreams[0];
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    const std::shared_ptr<std::fstream>& outStream = outStreams[0];
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
        if (!inStream->read(reinterpret_cast<char*>(&firstSample), sizeof(int16_t))) {
            throw CanNotRead();
        }
        inStream->seekg(startReadIndicator + position * 2);
        if (!inStream->read(reinterpret_cast<char*>(&secondSample), sizeof(int16_t))) {
            throw CanNotRead();
        }

        result = static_cast<int16_t>(static_cast<float>(firstSample) + static_cast<float>(secondSample) * decay);
        if (!outStream->write(reinterpret_cast<const char*>(&result), sizeof(int16_t))) {
            throw CanNotWrite();
        }
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}
