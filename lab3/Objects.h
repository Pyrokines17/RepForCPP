#pragma once

#include <algorithm>
#include <fstream>
#include <sstream>

#include "SupFuncs.h"

class GameObject {
protected:
    int weight,
        height;
    int healthPoints,
        gamePoints;

public:
    GameObject(int weight, int height, int healthPoints, int gamePoints);

    [[nodiscard]] int getW() const {return weight;};
    [[nodiscard]] int getH() const {return height;};
    [[nodiscard]] int getHP() const {return healthPoints;};
    [[nodiscard]] int getGP() const {return gamePoints;};

    virtual char getDir() {return 0;};
    virtual int getKit() {return 0;};
    virtual int getBul() {return 0;};
    virtual int getAF() {return 0;};
    virtual int getMode() {return 0;};

    virtual void addObj() {};

    void HPChange(int count) {healthPoints += count;};

    virtual char action(int c, const std::vector<GameObject*>& objects) = 0;
    virtual void draw(const std::vector<int>& pairs, int c) = 0;

    ~GameObject() = default;
};

class Bullet : public GameObject {
    steady_clock_t last_time;
    char direction;

public:
    Bullet(int weight, int height, steady_clock_t last_time, char direction);

    char getDir() override {return direction;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Player : public GameObject {
    int weightOfBorder,
        heightOfBorder;
    int countOfBullets;
    std::vector<std::string> body;
    std::vector<std::string> parts;
    int altFire,
        mode;

    static int checkStep(const std::vector<GameObject*>& objects, int futWeight, int futHeight);

public:
    Player(int weight, int height, int hp, int weightOfBorder, int heightOfBorder, int cob, int altFire, int mode);

    int getBul() override {return countOfBullets;};
    int getAF() override {return altFire;};
    int getMode() override {return mode;};

    void addObj() override {altFire = 1;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Gun : public GameObject {
    steady_clock_t last_time;
    std::vector<std::string> parts;
public:
    Gun(int weight, int height, steady_clock_t last_time);

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};
