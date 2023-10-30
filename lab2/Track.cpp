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

    readRiff(*inStream);
    readFmt(*inStream);
    readData(*inStream);
}

void Track::readRiff(std::ifstream& file) {
    // Read RIFF chunk

    FOURCC fourcc = 0;
    file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));
    BaseChunk riffChunk(fourcc);
    file.read(reinterpret_cast<char*>(&riffChunk.chunkSize), sizeof(uint32_t));
    header.riff = std::make_shared<BaseChunk>(riffChunk);

    // Read WAVE FOURCC

    file.read(reinterpret_cast<char*>(&fourcc), sizeof(FOURCC));
    header.waveFcc = fourcc;
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

void Track::copyData(void) {
    uint32_t startReadIndicator = static_cast<uint32_t>(inStream->tellg());
    uint32_t startWriteIndicator = static_cast<uint32_t>(outStream->tellp());
    uint32_t position = 0;
    int8_t byte = 0;

    while (position < header.data->chunkSize) {
        inStream->read(reinterpret_cast<char*>(&byte), sizeof(byte));
        outStream->write(reinterpret_cast<const char*>(&byte), sizeof(byte));
        position++;
    }

    inStream->seekg(startReadIndicator);
    outStream->seekp(startWriteIndicator);
}

void Track::writeRiff(std::fstream& file) {
    file.write(reinterpret_cast<const char*>(&header.riff->chunkId), sizeof(FOURCC));
    file.write(reinterpret_cast<const char*>(&header.riff->chunkSize), sizeof(uint32_t));

    file.write(reinterpret_cast<const char*>(&header.waveFcc), sizeof(FOURCC));
}

void Track::writeFmt(std::fstream& file) {
    file.write(reinterpret_cast<const char*>(&header.fmt_->chunkId), sizeof(FOURCC));
    file.write(reinterpret_cast<const char*>(&header.fmt_->chunkSize), sizeof(uint32_t));

    file.write(reinterpret_cast<const char*>(&header.fmtData->formatTag), sizeof(uint16_t));
    file.write(reinterpret_cast<const char*>(&header.fmtData->channels), sizeof(uint16_t));

    file.write(reinterpret_cast<const char*>(&header.fmtData->samplePerSec), sizeof(uint32_t));
    file.write(reinterpret_cast<const char*>(&header.fmtData->bytesPerSec), sizeof(uint32_t));

    file.write(reinterpret_cast<const char*>(&header.fmtData->blockAlign), sizeof(uint16_t));
    file.write(reinterpret_cast<const char*>(&header.fmtData->bitsPerSample), sizeof(uint16_t));
}

void Track::writeData(std::fstream& file) {
    file.write(reinterpret_cast<const char*>(&header.data->chunkId), sizeof(FOURCC));
    file.write(reinterpret_cast<const char*>(&header.data->chunkSize), sizeof(uint32_t));
}

int Track::getFinish() {
    return header.data->chunkSize;
}

std::shared_ptr<std::ifstream> Track::getInStr() {
    return inStream;
}

std::shared_ptr<std::fstream> Track::getOutStr() {
    return outStream;
}