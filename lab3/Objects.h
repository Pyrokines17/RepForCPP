#pragma once

#include <algorithm>
#include <fstream>
#include "SupFuncs.h"

class GameObject {
public:
    virtual int getW() = 0;
    virtual int getH() = 0;
    virtual int getHP() = 0;
    virtual int getGP() = 0;

    virtual void HPChange(int count) = 0;

    virtual char action(int c, const std::vector<GameObject*>& objects) = 0;
    virtual void draw(const std::vector<int>& pairs, int c) = 0;

    ~GameObject() = default;
};

class Bullet : public GameObject {
    int weight,
        height;
    int healthPoints,
        gamePoints;
    steady_clock_t last_time;
    char direction;

public:
    Bullet(int weight, int height, steady_clock_t last_time, char direction);

    int getW() override {return weight;};
    int getH() override {return height;};
    int getHP() override {return healthPoints;};
    int getGP() override {return gamePoints;};

    void HPChange(int count) override {healthPoints += count;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Player : public GameObject {
    int weight,
        height;
    int healthPoints,
        gamePoints;
    int weightOfBorder,
        heightOfBorder;
    int anim;
    std::vector<std::string> body;
    std::vector<std::string> parts;

    static int checkStep(const std::vector<GameObject*>& objects, int futWeight, int futHeight);

public:
    Player(int weight, int height, int weightOfBorder, int heightOfBorder);

    int getW() override {return weight;};
    int getH() override {return height;};
    int getHP() override {return healthPoints;};
    int getGP() override {return gamePoints;};

    void HPChange(int count) override {healthPoints += count;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Enemy : public GameObject {
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
    Enemy(int weight, int health, steady_clock_t last_time, int weightOfBorder, int heightOfBorder);

    int getW() override {return weight;};
    int getH() override {return height;};
    int getHP() override {return healthPoints;};
    int getGP() override {return gamePoints;};

    void HPChange(int count) override {healthPoints += count;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Block : public GameObject {
    int weight,
        height;
    int healthPoints,
        gamePoints,
        kit;

public:
    Block(int weight, int height, int kit);

    int getW() override {return weight;};
    int getH() override {return height;};
    int getHP() override {return healthPoints;};
    int getGP() override {return gamePoints;};

    void HPChange(int count) override {healthPoints += count;};

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Map {
    int weight,
        height;
    int countOfEnemy;
    [[maybe_unused]] int countOfBlock;
    std::vector<GameObject*> objects;

public:
    Map(int weight, int height, int countOfEnemy, int countOfBlock);

    void drawBorders() const;
    void printStat(int score) const;
    static void drawTable(std::ifstream& ifile);

    void actionOfObj(int c);
    void drawObj(const std::vector<int>& pairs, int c);

    void init(const std::vector<int>& parameters, steady_clock_t last_time, char direction, const std::string& name);

    int countingOfRes(int& score);
    void erase();

    ~Map() = default;
};