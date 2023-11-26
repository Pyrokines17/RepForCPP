#include "Objects.h"

Bullet::Bullet(int weight, int height, steady_clock_t last_time, char direction) :
weight(weight), height(height), healthPoints(1), gamePoints(-1), last_time(last_time), direction(direction) {
}

char Bullet::action(int c, const std::vector<GameObject*>& objects) {
    static_cast<void>(c);

    for (auto & object : objects) {
        if (object != this) {
            if (weight == object->getW() and height == object->getH()){
                object->HPChange(-25);
                healthPoints--;
            }
        }
    }

    return 0;
}

void Bullet::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[2]));
    if ((now() - last_time) / 1ms > 250) {
        if (direction == 'w') {
            height--;
        } else if (direction == 's') {
            height++;
        } else if (direction == 'd') {
            weight++;
        } else if (direction == 'a') {
            weight--;
        }
        last_time = now();
    }
    out(height, weight, "*");
    attroff(COLOR_PAIR(pairs[2]));
}

Player::Player(int weight, int height, int weightOfBorder, int heightOfBorder) :
weight(weight), height(height), healthPoints(100), gamePoints(-100), weightOfBorder(weightOfBorder), heightOfBorder(heightOfBorder) {
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
        case KEY_LEFT: res = 'a'; break;
        case KEY_RIGHT: res = 'd'; break;
        case KEY_UP: res = 'w'; break;
        case KEY_DOWN: res = 's'; break;
        default: ;
    }

    flushinp();

    return res;
}

void Player::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[0]));
    weight = std::clamp(weight, 1, weightOfBorder-2);
    height = std::clamp(height, 1, heightOfBorder-2);
    out(height, weight, "P");
    attroff(COLOR_PAIR(pairs[0]));
}

Enemy::Enemy(int weight, int height, steady_clock_t last_time, int weightOfBorder, int heightOfBorder) :
weight(weight), height(height), healthPoints(25), gamePoints(25), weightOfBorder(weightOfBorder), heightOfBorder(heightOfBorder), last_time(last_time) {
}

int Enemy::checkStep(const std::vector<GameObject*>& objects, int futWeight, int futHeight) {
    for (auto & object : objects) {
        if (object->getH() == futHeight and object->getW() == futWeight) {
            return 0;
        }
    }

    return 1;
}

char Enemy::action(int c, const std::vector<GameObject*>& objects) {
    static_cast<void>(c);
    char res = 0;

    if ((now() - last_time) / 1ms > 300) {
        int wt = objects[0]->getW(),
            ht = objects[0]->getH(),
            step = 0,
            shoot = 0;

        if (ht + 7 < height) {
            if (checkStep(objects, weight, height - 1)) {
                height--;
                step = 1;
            }
        }
        if (ht - 7 > height and step == 0) {
            if (checkStep(objects, weight, height + 1)) {
                height++;
                step = 1;
            }
        }
        if (wt - 10 > weight and step == 0) {
            if (checkStep(objects, weight + 1, height)) {
                weight++;
                step = 1;
            }
        }
        if (wt + 10 < weight and step == 0) {
            if (checkStep(objects, weight - 1, height)) {
                weight--;
            }
        }

        if (height == ht and weight > wt) {
            res = 'a';
            shoot = 1;
        }
        if (height == ht and weight < wt and shoot == 0) {
            res = 'd';
            shoot = 1;
        }
        if (weight == wt and height > ht and shoot == 0) {
            res = 'w';
            shoot = 1;
        }
        if (weight == wt and height < ht and shoot == 0) {
            res = 's';
        }

        last_time = now();
    }

    return res;
}

void Enemy::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[1]));
    weight = std::clamp(weight, 1, weightOfBorder-2);
    height = std::clamp(height, 1, heightOfBorder-2);
    out(height, weight, "E");
    attroff(COLOR_PAIR(pairs[1]));
}

Block::Block(int weight, int height, int kit) :
weight(weight), height(height), healthPoints(9999), gamePoints(0), kit(kit) {
}

char Block::action(int c, const std::vector<GameObject*>& objects) {
    if (c == 'e' and kit == 1 and objects[0]->getH() - 1 == height and objects[0]->getW() == weight) {
        objects[0]->HPChange(25);
        kit--;
    }

    return 0;
}

void Block::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[3]));
    out(height, weight, "B");
    attroff(COLOR_PAIR(pairs[3]));
}

Map::Map(int weight, int height, int countOfEnemy, int countOfBlock) :
weight(weight), height(height), countOfEnemy(countOfEnemy), countOfBlock(countOfBlock) {
}

void Map::drawBorders() const {
    for (int i = 1; i < weight; ++i) {
        out(0, i, "-");
        out(height - 1, i, "-");
    }

    for (int i = 1; i < height - 1; ++i) {
        out(i, 0, "|");
        out(i, weight - 1, "|");
    }

    out(0, 0, "+");
    out(0, weight - 1, "+");
    out(height - 1, 0, "+");
    out(height - 1, weight - 1, "+");
}

void Map::printStat(int score) const {
    const char* str = "HealthPoints: %d";
    print(str, objects[0]->getHP(), 1, weight);
    const char* str1 = "Enemy's: %lu";
    print(str1, countOfEnemy, 2, weight);
    const char* str2 = "Score: %d";
    print(str2, score, 3, weight);
}

void Map::actionOfObj(int c) {
    size_t border = objects.size();
    std::vector<int> coordinates = {0, 0};

    for (size_t i = 0; i < border; i++) {
        char res = objects[i]->action(c, objects);

        if (res > 0 and res != '#') {
            coordinates[0] = objects[i]->getW();
            coordinates[1] = objects[i]->getH();
            init(coordinates, now(), res, "bullet");
            border++;
        }

        if (res == '#') {
            coordinates[0] = objects[i]->getW();
            coordinates[1] = objects[i]->getH() + 1;
            init(coordinates, now(), '-', "kit");
            border++;
        }
    }
}

void Map::drawObj(const std::vector<int>& pairs) {
    for (auto & object : objects){
        object->draw(pairs);
    }
}

void Map::init(const std::vector<int>& parameters, steady_clock_t last_time, char direction, const std::string& name) {
    GameObject* a;

    if (name == "player") {
        a = new Player(weight / 2, height / 2, weight, height);
    } else if (name == "enemy") {
        a = new Enemy(parameters[0], parameters[1], last_time, weight, height);
    } else if (name == "block") {
        a = new Block(parameters[0], parameters[1], parameters[2]);
    } else if (name == "bullet") {
        if (direction == 'a') {
            a = new Bullet(parameters[0] - 1, parameters[1], last_time, direction);
        } else if (direction == 'd') {
            a = new Bullet(parameters[0] + 1, parameters[1], last_time, direction);
        } else if (direction == 'w') {
            a = new Bullet(parameters[0], parameters[1] - 1, last_time, direction);
        } else if (direction == 's') {
            a = new Bullet(parameters[0], parameters[1] + 1, last_time, direction);
        }
    }

    objects.push_back(a);
}

int Map::countingOfRes(int& score) {
    int endOfGame = 0;

    for (auto & object : objects) {
        if (object->getHP() == 0) {
            score += object->getGP();

            if (object->getGP() > 0) {
                countOfEnemy--;
            }

            if (countOfEnemy == 0) {
                endOfGame = 1;
            }

            if (object->getGP() == -100) {
                endOfGame = 2;
            }
        }
    }

    return endOfGame;
}

void Map::erase() {
    objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                           [this](GameObject *b) { return b->getH() < 1 or b->getW() < 1 or b->getH() > this->height - 2 or b->getW() > this->weight - 2 or b->getHP() <= 0; }),
            objects.end()
    );
}
