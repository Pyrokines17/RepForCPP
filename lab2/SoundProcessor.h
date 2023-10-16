#include <iostream>
#include <cstdint>
#include <memory>

typedef uint32_t FOURCC;

template <char ch0, char ch1, char ch2, char ch3> struct MakeFOURCC { enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) }; };

class soundProc {
	struct baseChunk {
		FOURCC chunkId;
		uint32_t chunkSize;

		baseChunk(FOURCC fourcc);
	};

	struct waveFormat {
		uint16_t formatTag; // WAVE data format (PCM data value is 1)
		uint16_t channels; // number of channels

		uint32_t samplePerSec; // sample rate
		uint32_t bytesPerSec; // rate (channels * samplePerSec * bitsPerSample / 8)

		uint16_t blockAlign; // audio data block
		uint16_t bitsPerSample; // quantized bits

		waveFormat();
	};

	struct waveHeader {
		std::shared_ptr<baseChunk> riff;
		FOURCC waveFcc;

		std::shared_ptr<baseChunk> fmt_;
		std::shared_ptr<waveFormat> fmtData;

		std::shared_ptr<baseChunk> data;

		waveHeader();
	};

	std::unique_ptr<waveHeader> header;

	std::shared_ptr<std::ifstream> inStream;
	std::shared_ptr<std::ofstream> outStream;

	int readRiff(std::ifstream& file);
	int readFmt(std::ifstream& file);
	void readData(std::ifstream& file); // check List or Data

	void writeRiff(std::ofstream& file);
	void writeFmt(std::ofstream& file);
	void writeData(std::ofstream& file);

public:
	soundProc();

	int readHead(std::string& name);
	void writeHead(std::string name);

	void mute(int time);
	void mix(soundProc&);
	void mixAlt(soundProc&);

};

