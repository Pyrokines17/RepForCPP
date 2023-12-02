#include "Map.h"
#include "Blocks.h"
#include "Enemys.h"

Map::Map(int weight, int height, int countOfEnemy, int countOfBlock) :
weight(weight), height(height), countOfEnemy(countOfEnemy), countOfBlock(countOfBlock),
gamePoints(0), countOfTacts(0) {
}

void Map::save() const {
    std::ofstream file;
    file.open("save.txt");

    if (!file.is_open()) {
        throw CanNotOpen();
    }

    file << countOfEnemy << " " << countOfBlock << " " << gamePoints << std::endl;

    for (auto & object : objects) {
        file << object->getW() << " " << object->getH() << " "
             << object->getHP() << " " << object->getGP();
        if (object->getGP() == 0) {
            file << " " << static_cast<int>(object->getDir());
        } else if (object->getGP() == 5) {
            file << " " << object->getKit();
        } else if (object->getGP() == -100 or object->getGP() == 25 or object->getGP() == 50) {
            file << " " << object->getBul();
            if (object->getGP() == -100) {
                file << " " << object->getAF();
                file << " " << object->getMode();
            }
        }
        file << std::endl;
    }

    file.close();
}

void Map::load() {
    std::ifstream file;
    file.open("save.txt", std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        countOfTacts = 25;
        return;
    }

    int param;
    std::string str;
    getline(file, str);

    if (file.bad() or file.fail()) {
        throw CanNotRead();
    }

    std::stringstream params(str);

    params >> param;
    countOfEnemy = param;
    params >> param;
    countOfBlock = param;
    params >> param;
    gamePoints = param;

    objects.clear();

    while (getline(file, str)) {
        GameObject* a;
        std::stringstream params1(str);
        std::vector<int> mass;

        while (!params1.eof()) {
            int param1;
            params1 >> param1;
            mass.push_back(param1);
        }

        if (!mass.empty()) {
            switch (mass[3]) {
                case 0: a = new Bullet(mass[0], mass[1], now(), static_cast<char>(mass[4])); break;
                case -100: a = new Player(mass[0], mass[1], mass[2], weight, height, mass[4], mass[5], mass[6]); break;
                case 5: a = new Block(mass[0], mass[1], mass[2], mass[4], now(), mass[3]); break;
                case 1: a = new AltBlock(mass[0], mass[1], mass[2], mass[4], now(), mass[3]); break;
                case 25: a = new Enemy(mass[0], mass[1], mass[2], now(), weight, height, mass[3], mass[4]); break;
                case 50: a = new AltEnemy(mass[0], mass[1], mass[2], now(), weight, height, mass[3], mass[4]); break;
                case 100: a = new Gun(mass[0], mass[1], now());
                default: break;
            }
        }

        if (!mass.empty()) {
            objects.push_back(a);
        }
    }

    file.close();
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

void Map::printStat() const {
    const char* str = "HealthPoints: %d";
    print(str, objects[0]->getHP(), 1, weight);
    str = "Enemy's: %lu";
    print(str, countOfEnemy, 2, weight);
    str = "Score: %d";
    print(str, gamePoints, 3, weight);
    str = "Bullets: %d";
    print(str, objects[0]->getBul(), 4, weight);
    str = "Mode: %d";
    print(str, objects[0]->getMode(), 5, weight);
}

void Map::drawTable(std::ifstream& ifile) {
    std::string str;
    int count = 0;

    while (std::getline(ifile, str)) {
        if (ifile.bad() or ifile.fail()) {
            throw CanNotRead();
        }

        out(count + 1, 1, str);
        count++;
    }
}

void Map::printText() {
    if (countOfTacts > 0) {
        std::string str = "Save not found :(";
        out(height - 1, weight / 2, str);
        countOfTacts--;
    }
}

void Map::actionOfObj(int c) {
    size_t border = objects.size();
    std::vector<int> coordinates = {0, 0};

    for (size_t i = 0; i < border; i++) {
        char res = objects[i]->action(c, objects);

        if (res > 0) {
            coordinates[0] = objects[i]->getW();
            coordinates[1] = objects[i]->getH();
            init(coordinates, now(), res, "bullet");
            border++;
        }
    }
}

void Map::drawObj(const std::vector<int>& pairs, int c) {
    for (auto & object : objects) {
        object->draw(pairs, c);
    }
}

void Map::init(const std::vector<int>& parameters, steady_clock_t last_time, char direction, const std::string& name) {
    if (name == "player") {
        auto a = new Player(weight / 2, height / 2, 100, weight, height, 6, 0, 1);
        objects.push_back(a);
    } else if (name == "enemy") {
        auto a = new Enemy(parameters[0], parameters[1], 25, last_time, weight, height, 25, 6);
        objects.push_back(a);
    } else if (name == "altEnemy") {
        auto a = new AltEnemy(parameters[0], parameters[1], 50, last_time, weight, height, 50, 2);
        objects.push_back(a);
    } else if (name == "block") {
        auto a = new Block(parameters[0], parameters[1], 2000, parameters[2], now(), 5);
        objects.push_back(a);
    } else if (name == "altBlock") {
        auto a = new AltBlock(parameters[0], parameters[1], 200, parameters[2], now(), 1);
        objects.push_back(a);
    } else if (name == "gun") {
      auto a = new Gun(parameters[0], parameters[1], now());
      objects.push_back(a);
    } else if (name == "bullet") {
        switch (direction) {
            case 'a': {
                auto a = new Bullet(parameters[0] - 2, parameters[1], last_time, direction);
                objects.push_back(a);
            } break;
            case 'd': {
                auto a = new Bullet(parameters[0] + 2, parameters[1], last_time, direction);
                objects.push_back(a);
            } break;
            case 'w': {
                auto a = new Bullet(parameters[0], parameters[1] - 2, last_time, direction);
                objects.push_back(a);
            } break;
            case 's': {
                auto a = new Bullet(parameters[0], parameters[1] + 2, last_time, direction);
                objects.push_back(a);
            } break;

            case 'j': {
                GameObject* a;
                a = new Bullet(parameters[0] - 2, parameters[1] - 1, last_time, '7');
                objects.push_back(a);
                a = new Bullet(parameters[0] - 2, parameters[1], last_time, 'a');
                objects.push_back(a);
                a = new Bullet(parameters[0] - 2, parameters[1] + 1, last_time, '1');
                objects.push_back(a);
            } break;
            case 'l': {
                GameObject* a;
                a = new Bullet(parameters[0] + 2, parameters[1] - 1, last_time, '9');
                objects.push_back(a);
                a = new Bullet(parameters[0] + 2, parameters[1], last_time, 'd');
                objects.push_back(a);
                a = new Bullet(parameters[0] + 2, parameters[1] + 1, last_time, '3');
                objects.push_back(a);
            } break;
            case 'i': {
                GameObject* a;
                a = new Bullet(parameters[0] - 1, parameters[1] - 2, last_time, '7');
                objects.push_back(a);
                a = new Bullet(parameters[0], parameters[1] - 2, last_time, 'w');
                objects.push_back(a);
                a = new Bullet(parameters[0] + 1, parameters[1] - 2, last_time, '9');
                objects.push_back(a);
            } break;
            case 'k': {
                GameObject* a;
                a = new Bullet(parameters[0] - 1, parameters[1] + 2, last_time, '1');
                objects.push_back(a);
                a = new Bullet(parameters[0], parameters[1] + 2, last_time, 's');
                objects.push_back(a);
                a = new Bullet(parameters[0] + 1, parameters[1] + 2, last_time, '3');
                objects.push_back(a);
            } break;

            default: break;
        }
    }
}

int Map::countingOfRes(int& score) {
    int endOfGame = 0;

    for (auto & object : objects) {
        if (object->getHP() <= 0) {
            gamePoints += object->getGP();

            if (object->getGP() == 25 or object->getGP() == 50) {
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

    score = gamePoints;

    return endOfGame;
}

void Map::erase() {
    objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                           [this](GameObject *b) { return b->getH() < 1 or b->getW() < 1 or
                           b->getH() > this->height - 2 or b->getW() > this->weight - 2 or
                           b->getHP() <= 0; }),
            objects.end()
    );
}