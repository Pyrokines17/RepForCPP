#include "Track.h"

Track::BaseChunk::BaseChunk(FOURCC fourcc) :chunkId(fourcc), chunkSize(0) {
}

Track::WaveFormat::WaveFormat() :formatTag(1), channels(0), samplePerSec(0), bytesPerSec(0), blockAlign(0), bitsPerSample(0) {
}

Track::WaveHeader::WaveHeader() :waveFcc(0) {
    riff = nullptr;
    fmt_ = nullptr;
    fmtData = nullptr;
    data = nullptr;
}

Track::Track() {
    inStream = std::make_shared<std::ifstream>();
    outStream = std::make_shared<std::fstream>();
}

void Track::readHead(std::string& name) {
    inStream->open(name, std::ios::in | std::ios::binary);

    if (!inStream->is_open()) {
        throw CanNotOpen();
    }

    readRiff(*inStream);
    readFmt(*inStream);
    readData(*inStream);
}

void Track::readRiff(std::ifstream& file) {
    // Read RIFF chunk
    FOURCC fourcc = 0;

    if (!file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC))) {
        throw CanNotRead();
    }

    if (fourcc != MakeFOURCC<'R', 'I', 'F', 'F'>::value) {
        throw RIFFNotFound();
    }

    BaseChunk riffChunk(fourcc);

    if (!file.read(reinterpret_cast<char*>(&riffChunk.chunkSize), sizeof(uint32_t))) {
        throw CanNotRead();
    }

    header.riff = std::make_shared<BaseChunk>(riffChunk);
    // Read WAVE FOURCC

    if (!file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC))) {
        throw CanNotRead();
    }

    header.waveFcc = fourcc;
}

void Track::readFmt(std::ifstream& file) {
    // Read fmt_ chunk
    FOURCC fourcc = 0;

    if (!file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC))) {
        throw CanNotRead();
    }

    while (fourcc != MakeFOURCC<'f', 'm', 't', ' '>::value) { // Determine if it is not fmt_
        uint32_t exSize = 0;

        if (!file.read(reinterpret_cast<char*>(&exSize), sizeof(uint32_t))) {
            throw CanNotRead();
        }

        uint32_t pos = static_cast<uint32_t>(file.tellg());
        file.seekg(pos + exSize);

        if (!file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC))) {
            throw CanNotRead();
        }
    }

    BaseChunk fmt_Chunk(fourcc);

    if (!file.read(reinterpret_cast<char*>(&fmt_Chunk.chunkSize), sizeof(uint32_t))) {
        throw CanNotRead();
    }

    header.fmt_ = std::make_shared<BaseChunk>(fmt_Chunk);
    // Read fmt_ data

    WaveFormat fmt_data;
    if (!file.read(reinterpret_cast<char*>(&fmt_data.formatTag), sizeof(uint16_t))) {
        throw CanNotRead();
    }
    if (!file.read(reinterpret_cast<char*>(&fmt_data.channels), sizeof(uint16_t))) {
        throw CanNotRead();
    }
    if (!file.read(reinterpret_cast<char*>(&fmt_data.samplePerSec), sizeof(uint32_t))) {
        throw CanNotRead();
    }
    if (!file.read(reinterpret_cast<char*>(&fmt_data.bytesPerSec), sizeof(uint32_t))) {
        throw CanNotRead();
    }
    if (!file.read(reinterpret_cast<char*>(&fmt_data.blockAlign), sizeof(uint16_t))) {
        throw CanNotRead();
    }
    if (!file.read(reinterpret_cast<char*>(&fmt_data.bitsPerSample), sizeof(uint16_t))) {
        throw CanNotRead();
    }

    header.fmtData = std::make_shared<WaveFormat>(fmt_data);
}

void Track::readData(std::ifstream& file) {
    FOURCC fourcc = 0;

    if (!file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC))) {
        throw CanNotRead();
    }

    while (fourcc != MakeFOURCC<'d', 'a', 't', 'a'>::value) { // Determine if it is not data
        uint32_t exSize = 0;

        if (!file.read(reinterpret_cast<char*>(&exSize), sizeof(uint32_t))) {
            throw CanNotRead();
        }

        uint32_t pos = static_cast<uint32_t>(file.tellg());
        file.seekg(pos + exSize);

        if (!file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC))) {
            throw CanNotRead();
        }
    }

    BaseChunk dataChunk(fourcc);

    if (!file.read(reinterpret_cast<char*>(&dataChunk.chunkSize), sizeof(uint32_t))) {
        throw CanNotRead();
    }

    header.data = std::make_shared<BaseChunk>(dataChunk);
}

void Track::writeHead(const std::string& name) {
    outStream->open(name, std::ios::out | std::ios::binary);

    if (!outStream->is_open()) {
        throw CanNotOpen();
    }

    writeRiff(*outStream);
    writeFmt(*outStream);
    writeData(*outStream);
}

void Track::copyData() {
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());
    uint32_t position = 0;
    int8_t byte = 0;

    while (position < header.data->chunkSize) {
        if (!inStream->read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
            throw CanNotRead();
        }
        if (!outStream->write(reinterpret_cast<const char*>(&byte), sizeof(byte))) {
            throw CanNotWrite();
        }
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}

void Track::writeRiff(std::fstream& file) {
    if (!file.write(reinterpret_cast<const char*>(&header.riff->chunkId), sizeof(FOURCC))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.riff->chunkSize), sizeof(uint32_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.waveFcc), sizeof(FOURCC))) {
        throw CanNotWrite();
    }
}

void Track::writeFmt(std::fstream& file) {
    if (!file.write(reinterpret_cast<const char*>(&header.fmt_->chunkId), sizeof(FOURCC))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmt_->chunkSize), sizeof(uint32_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmtData->formatTag), sizeof(uint16_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmtData->channels), sizeof(uint16_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmtData->samplePerSec), sizeof(uint32_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmtData->bytesPerSec), sizeof(uint32_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmtData->blockAlign), sizeof(uint16_t))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.fmtData->bitsPerSample), sizeof(uint16_t))) {
        throw CanNotWrite();
    }
}

void Track::writeData(std::fstream& file) {
    if (!file.write(reinterpret_cast<const char*>(&header.data->chunkId), sizeof(FOURCC))) {
        throw CanNotWrite();
    }
    if (!file.write(reinterpret_cast<const char*>(&header.data->chunkSize), sizeof(uint32_t))) {
        throw CanNotWrite();
    }
}

int Track::getFinish() {
    return static_cast<int>(header.data->chunkSize);
}

std::shared_ptr<std::ifstream> Track::getInStr() {
    return inStream;
}

std::shared_ptr<std::fstream> Track::getOutStr() {
    return outStream;
}