#include <cstdint>
#include <memory>

#define FOURCC uint32_t

template <char ch0, char ch1, char ch2, char ch3> struct MakeFOURCC { enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) }; };

struct baseChunk {
	FOURCC chunkId;
	uint32_t chunkSize;

	baseChunk(FOURCC fourcc);
};

struct waveFormat {
	uint16_t formatTag; // WAVE data format (PCM data value is 1)
	uint16_t channels; // number of channels
	uint32_t samplePerSec; // sample rate
	uint32_t bytesPerSec; // rate (channels * sample_per_sec * bits_per_sample / 8)
	uint16_t blockAlign; // audio data block
	uint16_t bitsPerSample; // quantized bits
	uint16_t exSize; // size of the extension block

	waveFormat();

	waveFormat(uint16_t nbChannel, uint32_t sampleRate, uint16_t sampleBits);
};

struct waveHeader {
	std::shared_ptr<baseChunk> riff;
	FOURCC waveFcc;
	std::shared_ptr<baseChunk> fmt;
	std::shared_ptr<waveFormat> fmtData;
	std::shared_ptr<baseChunk> data;

	waveHeader();

	waveHeader(uint16_t nbChannel, uint32_t sampleRate, uint16_t sampleBits);
};

struct actions {
	void write(waveHeader& header, unsigned int sec);
	int read();
};