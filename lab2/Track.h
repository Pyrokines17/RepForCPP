#include <fstream>
#include <memory>

typedef uint32_t FOURCC;

template <char ch0, char ch1, char ch2, char ch3> struct MakeFOURCC { enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) }; };

class Track {
	struct BaseChunk {
		FOURCC chunkId;
		uint32_t chunkSize;

		explicit BaseChunk(FOURCC fourcc);
	};

	struct WaveFormat {
		uint16_t formatTag; // WAVE data format (PCM data value is 1)
		uint16_t channels; // number of channels

		uint32_t samplePerSec; // sample rate
		uint32_t bytesPerSec; // rate (channels * samplePerSec * bitsPerSample / 8)

		uint16_t blockAlign; // audio data block
		uint16_t bitsPerSample; // quantized bits

		WaveFormat();
	};

	struct WaveHeader {
		std::shared_ptr<BaseChunk> riff;
		FOURCC waveFcc;

		std::shared_ptr<BaseChunk> fmt_;
		std::shared_ptr<WaveFormat> fmtData;

		std::shared_ptr<BaseChunk> data;

		WaveHeader();
	};

	WaveHeader header; 

	std::shared_ptr<std::ifstream> inStream;
	std::shared_ptr<std::fstream> outStream;

	void readRiff(std::ifstream& file);
	void readFmt(std::ifstream& file);
	void readData(std::ifstream& file);

	void writeRiff(std::fstream& file);
	void writeFmt(std::fstream& file);
	void writeData(std::fstream& file);

public:
	Track();

	void readHead(std::string& name);
	void writeHead(const std::string& name);
	void copyData(void);

	int getFinish();
    std::shared_ptr<std::ifstream> getInStr();
    std::shared_ptr<std::fstream> getOutStr();

};