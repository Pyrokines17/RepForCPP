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

class Player : public GameObject { //ToDo altFire() and reload()
    int weightOfBorder,
        heightOfBorder;
    int anim;
    std::vector<std::string> body;
    std::vector<std::string> parts;

    static int checkStep(const std::vector<GameObject*>& objects, int futWeight, int futHeight);

public:
    Player(int weight, int height, int hp, int weightOfBorder, int heightOfBorder);

    char action(int c, const std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs, int c) override;
};

class Map {
    int weight,
        height;
    int countOfEnemy;
    [[maybe_unused]] int countOfBlock;
    int gamePoints;
    std::vector<GameObject*> objects;

public:
    Map(int weight, int height, int countOfEnemy, int countOfBlock);

    void save() const;
    void load();

    void drawBorders() const;
    void printStat() const;
    static void drawTable(std::ifstream& ifile);

    void actionOfObj(int c);
    void drawObj(const std::vector<int>& pairs, int c);

    void init(const std::vector<int>& parameters, steady_clock_t last_time, char direction, const std::string& name);

    int countingOfRes(int& score);
    void erase();

    ~Map() = default;
};