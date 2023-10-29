#include "Converters.h"

void Mute::convert(Track* tracklist, std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = tracklist[0].getInStr();
    std::shared_ptr<std::ofstream> outStream = tracklist[0].getOutStr();
    auto header = tracklist[0].getHeader();

    uint32_t startIndicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t beginBorder = sampleRate * bitsPerSample * parameters[0] / 8;
    uint32_t endBorder = sampleRate * bitsPerSample * parameters[1] / 8;

    while (position < beginBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    byte = 0;

    while (position < endBorder) {
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    uint32_t indicator = static_cast<uint32_t>(inStream->tellg());
    inStream->seekg(indicator + endBorder - beginBorder);

    while (position < header.data->chunkSize) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    inStream->seekg(startIndicator);
}

void Mix::convert(Track* tracklist, std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = tracklist[0].getInStr();
    std::shared_ptr<std::ofstream> outStream = tracklist[0].getOutStr();
    auto header = tracklist[0].getHeader();

    int numOfSecTrack = parameters[0];
    std::shared_ptr<std::ifstream> secInStream = tracklist[numOfSecTrack].getInStr();

    uint32_t indicator1 = static_cast<uint32_t>(inStream->tellg());
    uint32_t indicator2 = static_cast<uint32_t>(secInStream->tellg());
    uint32_t position = 0;
    int8_t byteOfFirst = 0;
    int8_t byteOfSecond = 0;
    int8_t result = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t startBorder = sampleRate * bitsPerSample * parameters[1] / 8;

    while (position < startBorder) {
        inStream->read(reinterpret_cast<char*>(&result), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int8_t));
        position++;
    }

    while (position < header.data->chunkSize) {
        inStream->read(reinterpret_cast<char*>(&byteOfFirst), sizeof(int8_t));
        secInStream->read(reinterpret_cast<char*>(&byteOfSecond), sizeof(int8_t));

        result = static_cast<int8_t>((byteOfFirst + byteOfSecond) / 2);

        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int8_t));
        position++;
    }

    inStream->seekg(indicator1);
    secInStream->seekg(indicator2);
}

void MixAlt::convert(Track* tracklist, std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = tracklist[0].getInStr();
    std::shared_ptr<std::ofstream> outStream = tracklist[0].getOutStr();
    auto header = tracklist[0].getHeader();

    int numOfSecTrack = parameters[0];
    std::shared_ptr<std::ifstream> secInStream = tracklist[numOfSecTrack].getInStr();

    uint32_t indicator1 = static_cast<uint32_t>(inStream->tellg());
    uint32_t indicator2 = static_cast<uint32_t>(secInStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t borderOfSec = sampleRate * bitsPerSample / 8;
    uint32_t startBorder = sampleRate * bitsPerSample * parameters[1] / 8;

    while (position < startBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    while (position < header.data->chunkSize) {
        for (uint32_t i = 0; i < borderOfSec; i++) {
            inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
            outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
            position++;
        }

        for (uint32_t i = 0; i < borderOfSec; i++) {
            secInStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
            outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
            position++;
        }
    }

    inStream->seekg(indicator1);
    secInStream->seekg(indicator2);
}

void Slowed::convert(Track* tracklist, std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = tracklist[0].getInStr();
    std::shared_ptr<std::ofstream> outStream = tracklist[0].getOutStr();
    auto header = tracklist[0].getHeader();

    uint32_t indicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t beginBorder = sampleRate * bitsPerSample * parameters[0] / 8;
    uint32_t endBorder = sampleRate * bitsPerSample * parameters[1] / 8;

    while (position < beginBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    while (position < endBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        byte = static_cast<int8_t>(byte / static_cast<int8_t>(parameters[2]));

        for (int i = 0; i < parameters[2]; i++) {
            outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        }

        position++;
    }

    while (position < header.data->chunkSize) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    inStream->seekg(indicator);
}

void Reverb::convert(Track* tracklist, std::vector<int>& parameters) {
    std::shared_ptr<std::ifstream> inStream = tracklist[0].getInStr();
    std::shared_ptr<std::ofstream> outStream = tracklist[0].getOutStr();
    auto header = tracklist[0].getHeader();

    uint32_t indicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;
    int8_t result = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t beginBorder = sampleRate * bitsPerSample * parameters[0] / 8;
    uint32_t endBorder = sampleRate * bitsPerSample * parameters[1] / 8;
    uint32_t delay = delayMilliseconds * sampleRate * bitsPerSample / 8000;

    while (position < beginBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    unsigned int i = 0;
    float decay = 1.0f / static_cast<float>(parameters[2]);
    auto* buffer = new int8_t[delay];

    while (position < beginBorder + delay) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        buffer[i] = byte;
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
        i++;
    }

    i = 0;

    while (position < endBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        result = static_cast<int8_t>(static_cast<float>(byte) + static_cast<float>(buffer[i]) * decay);
        buffer[i] = result;
        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int8_t));
        position++;

        if (i == delay - 1) {
            i = 0;
        }
        else {
            i++;
        }
    }

    while (position < header.data->chunkSize) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    delete[] buffer;
    inStream->seekg(indicator);
}
