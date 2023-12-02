#include "Objects.h"

GameObject::GameObject(int weight, int height, int healthPoints, int gamePoints) :
weight(weight), height(height), healthPoints(healthPoints), gamePoints(gamePoints) {
}

Bullet::Bullet(int weight, int height, steady_clock_t last_time, char direction) :
GameObject(weight, height, 1, 0), last_time(last_time), direction(direction) {
}

char Bullet::action(int c, const std::vector<GameObject*>& objects) {
    static_cast<void>(c);

    for (auto & object : objects) {
        if (object != this and object->getGP() != 0) {
            if (weight >= object->getW() - 1 and weight <= object->getW() + 1 and
                height >= object->getH() - 1 and height <= object->getH() + 1) {
                object->HPChange(-15);
                healthPoints--;
            }
        }
    }

    return 0;
}

void Bullet::draw(const std::vector<int>& pairs, int c) {
    static_cast<void>(c);
    attron(COLOR_PAIR(pairs[2]));

    if ((now() - last_time) / 1ms > 150) {
        switch (direction) {
            case 'w': height--; break;
            case 's': height++; break;
            case 'a': weight--; break;
            case 'd': weight++; break;
            case '1': weight--; height++; break;
            case '3': weight++; height++; break;
            case '7': weight--; height--; break;
            case '9': weight++; height--; break;
            default: break;
        }
        last_time = now();
    }
    out(height, weight, "*");

    attroff(COLOR_PAIR(pairs[2]));
}

Player::Player(int weight, int height, int hp, int weightOfBorder, int heightOfBorder, int cob, int altFire, int mode) :
GameObject(weight, height, hp, -100), weightOfBorder(weightOfBorder), heightOfBorder(heightOfBorder),
countOfBullets(cob), altFire(altFire), mode(mode) {
    body.emplace_back("(@)");
    body.emplace_back("!|\\");
    body.emplace_back("/ |");

    parts.emplace_back("/|!");
    parts.emplace_back("| \\");
}

int Player::checkStep(const std::vector<GameObject*>& objects, int futWeight, int futHeight) {
    for (auto & object : objects) {
        if (object->getH() == futHeight and object->getW() == futWeight) {
            return 0;
        }
    }

    return 1;
}

char Player::action(int c, const std::vector<GameObject*>& objects) {
    char res = 0;

    switch(c) {
        case 'a': if (checkStep(objects, weight - 1, height)) {weight--;} break;
        case 'd': if (checkStep(objects, weight + 1, height)) {weight++;} break;
        case 'w': if (checkStep(objects, weight, height - 1)) {height--;} break;
        case 's': if (checkStep(objects, weight, height + 1)) {height++;} break;
        case 'r': if (countOfBullets < 12)
        {if (mode == 1) {countOfBullets++;} else {countOfBullets += 3;}} break;
        case KEY_LEFT: if (countOfBullets > 0)
        {if (mode == 1) {res = 'a'; countOfBullets--;} else {res = 'j'; countOfBullets -= 3;}} break;
        case KEY_RIGHT: if (countOfBullets > 0)
        {if (mode == 1) {res = 'd'; countOfBullets--;} else {res = 'l'; countOfBullets -= 3;}} break;
        case KEY_UP: if (countOfBullets > 0)
        {if (mode == 1) {res = 'w'; countOfBullets--;} else {res = 'i'; countOfBullets -= 3;}} break;
        case KEY_DOWN: if (countOfBullets > 0)
        {if (mode == 1) {res = 's'; countOfBullets--;} else {res = 'k'; countOfBullets -= 3;}} break;
        case 'x': if (altFire == 1) {mode = mode == 1 ? 3 : 1;} break;
        default: break;
    }

    countOfBullets = std::clamp(countOfBullets, 0, 12);

    flushinp();

    return res;
}

void Player::draw(const std::vector<int>& pairs, int c) {
    attron(COLOR_PAIR(pairs[0]));

    weight = std::clamp(weight, 2, weightOfBorder - 3);
    height = std::clamp(height, 1, heightOfBorder - 3);

    if (c == 'w' or c == 'a' or c == 's' or c == 'd') {
        std::swap(body[1], parts[0]);
        std::swap(body[2], parts[1]);
    }

    out(height - 1, weight - 1, body[0]);
    out(height, weight - 1, body[1]);
    out(height + 1, weight - 1, body[2]);

    attroff(COLOR_PAIR(pairs[0]));
}

Gun::Gun(int weight, int height, steady_clock_t last_time) :
GameObject(weight, height, 9999, 100), last_time(last_time) {
    parts.emplace_back("t?j");
    parts.emplace_back("");
}

char Gun::action(int c, const std::vector<GameObject *> &objects) {
    int subW = objects[0]->getW() - weight;
    int subH = objects[0]->getH() - height;

    if (c == 'e' and (subW * subW + subH * subH <= 2)) {
        objects[0]->addObj();
        healthPoints = 0;
    }

    return 0;
}

void Gun::draw(const std::vector<int> &pairs, int c) {
    static_cast<void>(c);
    attron(COLOR_PAIR(pairs[4]));

    if ((now() - last_time) / 1ms > 600) {
        std::swap(parts[0], parts[1]);
        last_time = now();
    }

    out(height - 1, weight - 1, parts[0]);
    out(height, weight - 1, parts[1]);

    attroff(COLOR_PAIR(pairs[4]));
}
