#include "Track.h"

Track::BaseChunk::BaseChunk(FOURCC fourcc) :chunkId(fourcc) {
    chunkSize = 0;
};

Track::WaveFormat::WaveFormat() {
    formatTag = 1; // PCM format data
    channels = 0;

    samplePerSec = 0;
    bytesPerSec = 0;

    blockAlign = 0;
    bitsPerSample = 0;
}

Track::WaveHeader::WaveHeader() {
    riff = nullptr;
    waveFcc = 0;

    fmt_ = nullptr;
    fmtData = nullptr;

    data = nullptr;
}

Track::Track() {
    inStream = std::make_shared<std::ifstream>();
    outStream = std::make_shared<std::ofstream>();
}

int Track::readHead(std::string& name) {
    inStream->open(name, std::ios::in | std::ios::binary);

    if (readRiff(*inStream) == false) {
        return false;
    }

    readFmt(*inStream);
    readData(*inStream);

    return true;
}

int Track::readRiff(std::ifstream& file) {
    // Read RIFF chunk

    FOURCC fourcc = 0;
    file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));

    if (fourcc != MakeFOURCC<'R', 'I', 'F', 'F'>::value) { // Determine if it is RIFF
        return false;
    }

    BaseChunk riffChunk(fourcc);
    file.read(reinterpret_cast<char*>(&riffChunk.chunkSize), sizeof(uint32_t));
    header.riff = std::make_shared<BaseChunk>(riffChunk);

    // Read WAVE FOURCC

    file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));

    if (fourcc != MakeFOURCC<'W', 'A', 'V', 'E'>::value) {
        return false;
    }

    header.waveFcc = fourcc;

    return true;
}

void Track::readFmt(std::ifstream& file) {
    // Read fmt_ chunk

    FOURCC fourcc = 0;
    file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));

    while (fourcc != MakeFOURCC<'f', 'm', 't', ' '>::value) { // Determine if it is not fmt_
        uint32_t exSize = 0;
        file.read(reinterpret_cast<char*>(&exSize), sizeof(uint32_t));

        uint32_t pos = static_cast<uint32_t>(file.tellg());
        file.seekg(pos + exSize);

        file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));
    }

    BaseChunk fmt_Chunk(fourcc);
    file.read(reinterpret_cast<char*>(&fmt_Chunk.chunkSize), sizeof(uint32_t));
    header.fmt_ = std::make_shared<BaseChunk>(fmt_Chunk);

    // Read fmt_ data

    WaveFormat fmt_data;
    file.read(reinterpret_cast<char*>(&fmt_data.formatTag), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&fmt_data.channels), sizeof(uint16_t));

    file.read(reinterpret_cast<char*>(&fmt_data.samplePerSec), sizeof(uint32_t));
    file.read(reinterpret_cast<char*>(&fmt_data.bytesPerSec), sizeof(uint32_t));

    file.read(reinterpret_cast<char*>(&fmt_data.blockAlign), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&fmt_data.bitsPerSample), sizeof(uint16_t));
    header.fmtData = std::make_shared<WaveFormat>(fmt_data);
}

void Track::readData(std::ifstream& file) {
    FOURCC fourcc = 0;
    file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));

    while (fourcc != MakeFOURCC<'d', 'a', 't', 'a'>::value) { // Determine if it is not data
        uint32_t exSize = 0;
        file.read(reinterpret_cast<char*>(&exSize), sizeof(uint32_t));

        uint32_t pos = static_cast<uint32_t>(file.tellg());
        file.seekg(pos + exSize);

        file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));
    }

    BaseChunk dataChunk(fourcc);
    file.read(reinterpret_cast<char*>(&dataChunk.chunkSize), sizeof(uint32_t));
    header.data = std::make_shared<BaseChunk>(dataChunk);
}

void Track::writeHead(std::string name) {
    outStream->open(name, std::ios::out | std::ios::binary);

    writeRiff(*outStream);
    writeFmt(*outStream);
    writeData(*outStream);
}

void Track::writeRiff(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&header.riff->chunkId), sizeof(FOURCC));
    file.write(reinterpret_cast<const char*>(&header.riff->chunkSize), sizeof(uint32_t));

    file.write(reinterpret_cast<const char*>(&header.waveFcc), sizeof(FOURCC));
}

void Track::writeFmt(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&header.fmt_->chunkId), sizeof(FOURCC));
    file.write(reinterpret_cast<const char*>(&header.fmt_->chunkSize), sizeof(uint32_t));

    file.write(reinterpret_cast<const char*>(&header.fmtData->formatTag), sizeof(uint16_t));
    file.write(reinterpret_cast<const char*>(&header.fmtData->channels), sizeof(uint16_t));

    file.write(reinterpret_cast<const char*>(&header.fmtData->samplePerSec), sizeof(uint32_t));
    file.write(reinterpret_cast<const char*>(&header.fmtData->bytesPerSec), sizeof(uint32_t));

    file.write(reinterpret_cast<const char*>(&header.fmtData->blockAlign), sizeof(uint16_t));
    file.write(reinterpret_cast<const char*>(&header.fmtData->bitsPerSample), sizeof(uint16_t));
}

void Track::writeData(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&header.data->chunkId), sizeof(FOURCC));
    file.write(reinterpret_cast<const char*>(&header.data->chunkSize), sizeof(uint32_t));
}

void Track::mute(int begin, int end) {
    uint32_t startIndicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t beginBorder = sampleRate * bitsPerSample * begin / 8;
    uint32_t endBorder = sampleRate * bitsPerSample * end / 8;

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

void Track::mix(Track& secondTrack, int begin) {
    uint32_t indicator1 = static_cast<uint32_t>(inStream->tellg());
    uint32_t indicator2 = static_cast<uint32_t>(secondTrack.inStream->tellg());
    uint32_t position = 0;
    int8_t byteOfFirst = 0;
    int8_t byteOfSecond = 0;
    int8_t result = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t startBorder = sampleRate * bitsPerSample * begin / 8;

    while (position < startBorder) {
        inStream->read(reinterpret_cast<char*>(&result), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int8_t));
        position++;
    }

    while (position < header.data->chunkSize) {
        inStream->read(reinterpret_cast<char*>(&byteOfFirst), sizeof(int8_t));
        secondTrack.inStream->read(reinterpret_cast<char*>(&byteOfSecond), sizeof(int8_t));

        result = (byteOfFirst + byteOfSecond) / 2;

        outStream->write(reinterpret_cast<const char*>(&result), sizeof(int8_t));
        position++;
    }

    inStream->seekg(indicator1);
    secondTrack.inStream->seekg(indicator2);
}

void Track::mixAlt(Track& secondTrack, int begin) {
    uint32_t indicator1 = static_cast<uint32_t>(inStream->tellg());
    uint32_t indicator2 = static_cast<uint32_t>(secondTrack.inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t borderOfSec = sampleRate * bitsPerSample / 8;
    uint32_t startBorder = sampleRate * bitsPerSample * begin / 8;

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
            secondTrack.inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
            outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
            position++;
        }
    }

    inStream->seekg(indicator1);
    secondTrack.inStream->seekg(indicator2);
}

void Track::slowed(int begin, int end, int degree) { //mb rerelease...
    uint32_t indicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t beginBorder = sampleRate * bitsPerSample * begin / 8;
    uint32_t endBorder = sampleRate * bitsPerSample * end / 8;

    while (position < beginBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    while (position < endBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        byte /= static_cast<int8_t>(degree);

        for (int i = 0; i < degree; i++) {
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

void Track::reverb(int begin, int end, int degree) {
    uint32_t indicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t position = 0;
    int8_t byte = 0;
    int8_t result = 0;

    uint32_t sampleRate = header.fmtData->samplePerSec;
    uint16_t bitsPerSample = header.fmtData->bitsPerSample;

    uint32_t beginBorder = sampleRate * bitsPerSample * begin / 8;
    uint32_t endBorder = sampleRate * bitsPerSample * end / 8;
    uint32_t delay = delayMilliseconds * sampleRate * bitsPerSample / 8000;

    while (position < beginBorder) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(int8_t));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(int8_t));
        position++;
    }

    unsigned int i = 0;
    float decay = 1.0f / degree;
    int8_t* buffer = new int8_t[delay]; //mb to do read and write from one file

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
        result = byte + static_cast<int8_t>(buffer[i] * decay);
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