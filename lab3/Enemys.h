#pragma once

#include "Objects.h"

class Enemy : public GameObject {
protected:
    int weightOfBorder,
        heightOfBorder;
    steady_clock_t last_time;
    int countOfBullets,
        reload;
    std::vector<std::string> body;
    std::vector<std::string> parts;

    static int checkStep(const std::vector<GameObject*>& objects, int weight, int height);
    int look(const std::vector<GameObject*>& objects, char side);

public:
    Enemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp, int cob);

    int getBul() override {return countOfBullets;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class AltEnemy : public Enemy {
public:
    AltEnemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp, int cob);

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};