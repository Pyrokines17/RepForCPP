#include <iostream>
#include <fstream>
#include "SoundProcessor.h"

baseChunk::baseChunk(FOURCC fourcc) :chunkId(fourcc) {
    chunkSize = 0;
};

waveFormat::waveFormat() {
    formatTag = 1; // PCM format data
    exSize = 0; // don't use extesion field

    channels = 0;
    samplePerSec = 0;
    bytesPerSec = 0;
    blockAlign = 0;
    bitsPerSample = 0;
}

waveFormat::waveFormat(uint16_t nbChannel, uint32_t sampleRate, uint16_t sampleBits) :channels(nbChannel), samplePerSec(sampleRate), bitsPerSample(sampleBits) {
    formatTag = 0x01; // PCM format data
    bytesPerSec = channels * samplePerSec * bitsPerSample / 8; // code rate
    blockAlign = channels * bitsPerSample / 8;
    exSize = 0; // don't use extension field
};

waveHeader::waveHeader() {
    riff = nullptr;
    fmt = nullptr;

    fmtData = nullptr;
    data = nullptr;

    waveFcc = 0;
}

waveHeader::waveHeader(uint16_t nbChannel, uint32_t sampleRate, uint16_t sampleBits) {
    riff = std::make_shared<baseChunk>(MakeFOURCC<'R', 'I', 'F', 'F'>::value);
    fmt = std::make_shared<baseChunk>(MakeFOURCC<'f', 'm', 't', ' '>::value);

    fmt->chunkSize = 18;

    fmtData = std::make_shared<waveFormat>(nbChannel, sampleRate, sampleBits);
    data = std::make_shared<baseChunk>(MakeFOURCC<'d', 'a', 't', 'a'>::value);

    waveFcc = MakeFOURCC<'W', 'A', 'V', 'E'>::value;
}

void actions::write(waveHeader& header, unsigned int sec) {
    uint32_t length = header.fmtData->samplePerSec * sec * header.fmtData->bitsPerSample / 8;
    uint8_t* data = new uint8_t[length];
    memset(data, 0x80, length);

    //...
}

int actions::read() {
    std::unique_ptr<waveHeader> header = std::make_unique<waveHeader>();

    std::ifstream file("sth.wav", std::ios::in | std::ios::binary);

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
}