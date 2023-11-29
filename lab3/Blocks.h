#pragma once

#include "Objects.h"

class Block : public GameObject { //ToDo new block
    int weight,
        height;
    int healthPoints,
        gamePoints;
    int kit;

public:
    Block(int weight, int height, int hp, int kit);

    int getW() override {return weight;};
    int getH() override {return height;};
    int getHP() override {return healthPoints;};
    int getGP() override {return gamePoints;};
    char getDir() override {return 0;};
    int getKit() override {return kit;};

    void HPChange(int count) override {healthPoints += count;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};