#include <fstream>
#include "SoundProcessor.h"

soundProc::baseChunk::baseChunk(FOURCC fourcc) :chunkId(fourcc) {
    chunkSize = 0;
};

soundProc::waveFormat::waveFormat() {
    formatTag = 1; // PCM format data
    channels = 0;

    samplePerSec = 0;
    bytesPerSec = 0;

    blockAlign = 0;
    bitsPerSample = 0;
}

soundProc::waveHeader::waveHeader() {
    riff = nullptr;
    waveFcc = 0;

    fmt_ = nullptr;
    fmtData = nullptr;

    data = nullptr;
}

soundProc::soundProc() {
    header = std::make_unique<waveHeader>();

    inStream = std::make_shared<std::ifstream>();
    outStream = std::make_shared<std::ofstream>();
}

int soundProc::readHead(std::string& name) {
    inStream->open(name, std::ios::in | std::ios::binary);

    if (readRiff(*inStream) == false) {
        return false;
    }

    if (readFmt(*inStream) == false) {
        return false;
    }

    readData(*inStream);

    return true;
}

int soundProc::readRiff(std::ifstream& file) {
    // Read RIFF chunk

    FOURCC fourcc = 0;
    file.read((char*)&fourcc, sizeof(FOURCC));

    if (fourcc != MakeFOURCC<'R', 'I', 'F', 'F'>::value) { // Determine if it is RIFF
        return false;
    }

    baseChunk riffChunk(fourcc);
    file.read((char*)&riffChunk.chunkSize, sizeof(uint32_t));
    header->riff = std::make_shared<baseChunk>(riffChunk);

    // Read WAVE FOURCC

    file.read((char*)&fourcc, sizeof(FOURCC));

    if (fourcc != MakeFOURCC<'W', 'A', 'V', 'E'>::value) {
        return false;
    }

    header->waveFcc = fourcc;

    return true;
}

int soundProc::readFmt(std::ifstream& file) {
    // Read fmt_ chunk

    FOURCC fourcc = 0;
    file.read((char*)&fourcc, sizeof(FOURCC));

    if (fourcc != MakeFOURCC<'f', 'm', 't', ' '>::value) { // Determine if it is fmt_
        return false;
    }

    baseChunk fmt_Chunk(fourcc);
    file.read((char*)&fmt_Chunk.chunkSize, sizeof(uint32_t));
    header->fmt_ = std::make_shared<baseChunk>(fmt_Chunk);

    // Read fmt_ data

    waveFormat fmt_data;
    file.read((char*)&fmt_data.formatTag, sizeof(uint16_t));
    file.read((char*)&fmt_data.channels, sizeof(uint16_t));

    file.read((char*)&fmt_data.samplePerSec, sizeof(uint32_t));
    file.read((char*)&fmt_data.bytesPerSec, sizeof(uint32_t));

    file.read((char*)&fmt_data.blockAlign, sizeof(uint16_t));
    file.read((char*)&fmt_data.bitsPerSample, sizeof(uint16_t));
    header->fmtData = std::make_shared<waveFormat>(fmt_data);

    return true;
}

void soundProc::readData(std::ifstream& file) {
    FOURCC fourcc = 0;
    file.read((char*)&fourcc, sizeof(FOURCC));

    while (fourcc != MakeFOURCC<'d', 'a', 't', 'a'>::value) { // Determine if it is not data
        uint32_t exSize = 0;
        file.read((char*)&exSize, sizeof(uint32_t));

        uint32_t pos = static_cast<uint32_t>(file.tellg());
        file.seekg(pos + exSize);

        file.read((char*)&fourcc, sizeof(FOURCC));
    }

    baseChunk dataChunk(fourcc);
    file.read((char*)&dataChunk.chunkSize, sizeof(uint32_t));
    header->data = std::make_shared<baseChunk>(dataChunk);
}

void soundProc::writeHead(std::string name) {
    outStream->open(name, std::ios::out | std::ios::binary);

    writeRiff(*outStream);
    writeFmt(*outStream);
    writeData(*outStream);
}

void soundProc::writeRiff(std::ofstream& file) {
    file.write((const char*)&header->riff->chunkId, sizeof(FOURCC));
    file.write((const char*)&header->riff->chunkSize, sizeof(uint32_t));

    file.write((const char*)&header->waveFcc, sizeof(FOURCC));
}

void soundProc::writeFmt(std::ofstream& file) {
    file.write((const char*)&header->fmt_->chunkId, sizeof(FOURCC));
    file.write((const char*)&header->fmt_->chunkSize, sizeof(uint32_t));

    file.write((const char*)&header->fmtData->formatTag, sizeof(uint16_t));
    file.write((const char*)&header->fmtData->channels, sizeof(uint16_t));

    file.write((const char*)&header->fmtData->samplePerSec, sizeof(uint32_t));
    file.write((const char*)&header->fmtData->bytesPerSec, sizeof(uint32_t));

    file.write((const char*)&header->fmtData->blockAlign, sizeof(uint16_t));
    file.write((const char*)&header->fmtData->bitsPerSample, sizeof(uint16_t));
}

void soundProc::writeData(std::ofstream& file) {
    file.write((const char*)&header->data->chunkId, sizeof(FOURCC));
    file.write((const char*)&header->data->chunkSize, sizeof(uint32_t));
}

void soundProc::mute(int time) {
    uint32_t position = 0;
    int8_t byte = 0;

    uint32_t sampleRate = header->fmtData->samplePerSec;
    uint16_t bitsPerSample = header->fmtData->bitsPerSample;

    uint32_t border = sampleRate * bitsPerSample * time / 8;

    while (position < border) {
        outStream->write((const char*)&byte, sizeof(int8_t));
        position++;
    }

    uint32_t indicator = static_cast<uint32_t>(inStream->tellg());
    inStream->seekg(indicator + border);

    while (position < header->data->chunkSize) {
        inStream->read((char*)&byte, sizeof(int8_t));
        outStream->write((const char*)&byte, sizeof(int8_t));
        position++;
    }

    inStream->seekg(indicator);
}

void soundProc::mix(soundProc& secondTrack) {
    uint32_t position = 0;
    int8_t byteOfFirst = 0;
    int8_t byteOfSecond = 0;
    int8_t result = 0;

    uint32_t indicator1 = static_cast<uint32_t>(inStream->tellg());
    uint32_t indicator2 = static_cast<uint32_t>(secondTrack.inStream->tellg());

    while (position < header->data->chunkSize) {
        inStream->read((char*)&byteOfFirst, sizeof(int8_t));
        secondTrack.inStream->read((char*)&byteOfSecond, sizeof(int8_t));

        result = (byteOfFirst + byteOfSecond) / 2;

        outStream->write((const char*)&result, sizeof(int8_t));
        position++;
    }

    inStream->seekg(indicator1);
    secondTrack.inStream->seekg(indicator2);
}

void soundProc::mixAlt(soundProc& secondTrack) {
    uint32_t position = 0;
    int8_t byteOfFirst = 0;
    int8_t byteOfSecond = 0;

    uint32_t sampleRate = header->fmtData->samplePerSec;
    uint16_t bitsPerSample = header->fmtData->bitsPerSample;

    uint32_t border = sampleRate * bitsPerSample / 8;

    uint32_t indicator1 = static_cast<uint32_t>(inStream->tellg());
    uint32_t indicator2 = static_cast<uint32_t>(secondTrack.inStream->tellg());

    while (position < header->data->chunkSize) {
        for (uint32_t i = 0; i < border; i++) {
            inStream->read((char*)&byteOfFirst, sizeof(int8_t));
            outStream->write((const char*)&byteOfFirst, sizeof(int8_t));
            position++;
        }

        for (uint32_t i = 0; i < border; i++) {
            secondTrack.inStream->read((char*)&byteOfSecond, sizeof(int8_t));
            outStream->write((const char*)&byteOfSecond, sizeof(int8_t));
            position++;
        }
    }

    inStream->seekg(indicator1);
    secondTrack.inStream->seekg(indicator2);
}