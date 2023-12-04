#pragma once

#include "Objects.h"

class Block : public GameObject {
protected:
    int kit;
    steady_clock_t last_time;
    std::vector<std::string> parts;

public:
    Block(int weight, int height, int hp, int kit, steady_clock_t last_time, int gp);

    int getKit() override {return kit;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class AltBlock : public Block {
public:
    AltBlock(int weight, int height, int hp, int kit, steady_clock_t last_time, int gp);
};