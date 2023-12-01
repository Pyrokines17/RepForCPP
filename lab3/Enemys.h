#pragma once

#include "Objects.h"

class Enemy : public GameObject {
protected:
    int weightOfBorder,
        heightOfBorder;
    int anim;
    steady_clock_t last_time;
    std::vector<std::string> body;
    std::vector<std::string> parts;

    static int checkStep(const std::vector<GameObject*>& objects, int weight, int height);

public:
    Enemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp);

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class AltEnemy : public Enemy {
public:
    AltEnemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp);

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};