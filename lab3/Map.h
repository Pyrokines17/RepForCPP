#pragma once

#include "Objects.h"
#include "ListOfExcep.h"

class Map {
    int weight,
            height;
    int countOfEnemy,
            countOfBlock;
    int gamePoints;
    int countOfTacts;
    std::vector<GameObject*> objects;

public:
    Map(int weight, int height, int countOfEnemy, int countOfBlock);

    void save();
    void load();

    void drawBorders() const;
    void printStat() const;
    static void drawTable(std::ifstream& ifile);
    void printText(const std::string& str);

    void actionOfObj(int c);
    void drawObj(const std::vector<int>& pairs, int c);

    void init(const std::vector<int>& parameters, steady_clock_t last_time, char direction, const std::string& name);
    void initBul(const std::vector<int>& parameters, steady_clock_t last_time, char direction);

    int countingOfRes(int& score);
    void erase();

    ~Map() = default;
};