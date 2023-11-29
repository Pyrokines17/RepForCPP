#pragma once

#include "Objects.h"

class Enemy : public GameObject { //ToDo new enemy
    int weight,
        height;
    int healthPoints,
        gamePoints;
    int weightOfBorder,
        heightOfBorder;
    int anim;
    steady_clock_t last_time;
    std::vector<std::string> body;
    std::vector<std::string> parts;

    static int checkStep(const std::vector<GameObject*>& objects, int weight, int height);

public:
    Enemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder);

    int getW() override {return weight;};
    int getH() override {return height;};
    int getHP() override {return healthPoints;};
    int getGP() override {return gamePoints;};
    char getDir() override {return 0;};
    int getKit() override {return 0;};

    void HPChange(int count) override {healthPoints += count;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};