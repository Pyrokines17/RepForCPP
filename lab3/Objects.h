#include <algorithm>
#include "SupFuncs.h"

class Map;

class GameObject {
public:
    virtual int getW() = 0;
    virtual int getH() = 0;
    virtual char action(int c, std::vector<GameObject*>& objects) = 0;
    virtual void draw(const std::vector<int>& pairs) = 0;
    ~GameObject() = default;
};

class Bullet : public GameObject {
    int w,
        h;
    steady_clock_t last_time;
    char direction;
public:
    int getW() override {return w;};
    int getH() override {return h;};
    Bullet(int w, int h, steady_clock_t last_time, char direction);
    char action(int c, std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs) override;
};

class Player : public GameObject {
    int w,
        h,
        w1,
        h1;
public:
    int getW() override {return w;};
    int getH() override {return h;};
    Player(int w, int h, int w1, int h1);
    char action(int c, std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs) override;
};

class Enemy : public GameObject {
    int w,
        h,
        w1,
        h1;
    steady_clock_t last_time;
public:
    int getW() override {return w;};
    int getH() override {return h;};
    Enemy(int w, int h, steady_clock_t last_time, int w1, int h1);
    char action(int c, std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs) override;
};

class Block : public GameObject {
    int w,
        h;
public:
    int getW() override {return w;};
    int getH() override {return h;};
    Block(int w, int h);
    char action(int c, std::vector<GameObject*>& objects) override;
    void draw(const std::vector<int>& pairs) override;
};

class Map {
    int w,
        h,
        countOfEnemy,
        countOfBlock;
    std::vector<GameObject*> buf;
    std::vector<GameObject*> objects;
public:
    Map(int w, int h, int countOfEnemy, int countOfBlock);
    void drawBorders() const;
    void printStat() const;
    void actObj(int c);
    void drawObj(const std::vector<int>& pairs);
    void initPlayer();
    void initEnemy(int w, int h, steady_clock_t last_time, int w1, int h1);
    void initBlock(int w1, int h1);
    void initBul(int w1, int h1, steady_clock_t last_time, char direction);
    void erase();
    ~Map() = default;
};