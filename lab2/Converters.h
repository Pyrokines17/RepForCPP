#include <vector>
#include "Track.h"

class Converter {
public:
	virtual void convert(Track* tracklist, std::vector<int>& parameters) = 0;
	virtual ~Converter() = default;
};

class Mute : public Converter {
public:
	void convert(Track* tracklist, std::vector<int>& parameters) override;
};

class Mix : public Converter {
public:
	void convert(Track* tracklist, std::vector<int>& parameters) override;
};

class MixAlt : public Converter {
public:
	void convert(Track* tracklist, std::vector<int>& parameters) override;
};

class Slowed : public Converter { //(?)//
public:
	void convert(Track* tracklist, std::vector<int>& parameters) override;
};

class Reverb : public Converter { //(?)//
public:
	void convert(Track* tracklist, std::vector<int>& parameters) override;
};

class Call {
public:
    virtual Converter* factoryMethod() = 0;
};

class CallMute: public Call {
public:
    Converter* factoryMethod() override {return new Mute();}
};

class CallMix: public Call {
public:
    Converter* factoryMethod() override {return new Mix();}
};

class CallMixAlt: public Call {
public:
    Converter* factoryMethod() override {return new MixAlt();}
};

class CallSlowed: public Call {
public:
    Converter* factoryMethod() override {return new Slowed();}
};

class CallReverb: public Call {
public:
    Converter* factoryMethod() override {return new Reverb();}
};