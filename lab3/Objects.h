#pragma once

using steady_clock_t = std::chrono::time_point<std::chrono::steady_clock>;

class GameObject {
    int h,
        w;
    steady_clock_t last_time;
public:
    virtual void action() = 0;
    virtual void draw() = 0;
    ~GameObject() = default;
};

class Bullet : public GameObject {
public:
    void action() override;
    void draw() override;
};

class Player : public GameObject {
public:
    void action() override;
    void draw() override;
};

class Enemy : public GameObject {
public:
    void action() override;
    void draw() override;
};

class Wall : public GameObject {
public:
    void action() override;
    void draw() override;
};