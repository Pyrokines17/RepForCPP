#include "Track.h"

Track::BaseChunk::BaseChunk(FOURCC fourcc) :chunkId(fourcc) {
    chunkSize = 0;
}

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

void Track::writeHead(const std::string& name) {
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

Track::WaveHeader Track::getHeader() {
    return header;
}

std::shared_ptr<std::ifstream> Track::getInStr() {
    return inStream;
}

std::shared_ptr<std::ofstream> Track::getOutStr() {
    return outStream;
}