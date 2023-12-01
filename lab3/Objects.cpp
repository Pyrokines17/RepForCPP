#include "Objects.h"
#include "Enemys.h"
#include "Blocks.h"

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
                object->HPChange(-25);
                healthPoints--;
            }
        }
        if (object != this and object->getGP() == 0) {
            if (height == object->getH() and weight == object->getW()) {
                object->HPChange(-1);
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
        }
        last_time = now();
    }
    out(height, weight, "*");

    attroff(COLOR_PAIR(pairs[2]));
}

Player::Player(int weight, int height, int hp, int weightOfBorder, int heightOfBorder) :
GameObject(weight, height, hp, -100), weightOfBorder(weightOfBorder), heightOfBorder(heightOfBorder), anim(0) {
    body.emplace_back("(@)");
    body.emplace_back("/|\\");
    body.emplace_back("/ \\");

    parts.emplace_back("!|\\");
    parts.emplace_back("/ |");
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
        case 'a': if (checkStep(objects, weight - 1, height))
        {weight--;} break;
        case 'd': if (checkStep(objects, weight + 1, height))
        {weight++;} break;
        case 'w': if (checkStep(objects, weight, height - 1))
        {height--;} break;
        case 's': if (checkStep(objects, weight, height + 1))
        {height++;} break;
        case KEY_LEFT: res = 'a'; break;
        case KEY_RIGHT: res = 'd'; break;
        case KEY_UP: res = 'w'; break;
        case KEY_DOWN: res = 's'; break;
        default: ;
    }

    flushinp();

    return res;
}

void Player::draw(const std::vector<int>& pairs, int c) {
    attron(COLOR_PAIR(pairs[0]));

    weight = std::clamp(weight, 2, weightOfBorder - 3);
    height = std::clamp(height, 1, heightOfBorder - 3);

    if (c == 'w' or c == 'a' or c == 's' or c == 'd') {
        if (anim == 0) {
            body[1] = parts[0];
            body[2] = parts[1];
            anim = 1;
        } else {
            body[1] = parts[2];
            body[2] = parts[3];
            anim = 0;
        }
    }

    out(height - 1, weight - 1, body[0]);
    out(height, weight - 1, body[1]);
    out(height + 1, weight - 1, body[2]);

    attroff(COLOR_PAIR(pairs[0]));
}

Map::Map(int weight, int height, int countOfEnemy, int countOfBlock) :
weight(weight), height(height), countOfEnemy(countOfEnemy), countOfBlock(countOfBlock), gamePoints(0) {
}

void Map::save() const {
    std::ofstream file;
    file.open("save.txt");

    file << countOfEnemy << " " << countOfBlock << " " << gamePoints << std::endl;

    for (auto & object : objects) {
        file << object->getW() << " " << object->getH() << " "
            << object->getHP() << " " << object->getGP();
        if (object->getGP() == 0) {
            file << " " << static_cast<int>(object->getDir());
        } else if (object->getGP() == 5) {
            file << " " << object->getKit();
        }
        file << std::endl;
    }

    file.close();
}

void Map::load() {
    std::ifstream file;
    file.open("save.txt", std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        endwin();
        std::cerr << "File not open :(" << std::endl;
        exit(1);
    }

    int param;
    std::string str;
    getline(file, str);
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

        if (!mass.empty() and mass[3] == 0) {
            a = new Bullet(mass[0], mass[1], now(), static_cast<char>(mass[4]));
        } else if (!mass.empty() and mass[3] == -100) {
            a = new Player(mass[0], mass[1], mass[2], weight, height);
        } else if (!mass.empty() and mass[3] == 5) {
            a = new Block(mass[0], mass[1], mass[2], mass[4], now(), mass[3]);
        } else if(!mass.empty() and mass[3] == 1) {
            a = new AltBlock(mass[0], mass[1], mass[2], mass[4], now(), mass[3]);
        } else if (!mass.empty() and mass[3] == 25) {
            a = new Enemy(mass[0], mass[1], mass[2], now(), weight, height, mass[3]);
        } else if (!mass.empty() and mass[3] == 50) {
            a = new AltEnemy(mass[0], mass[1], mass[2], now(), weight, height, mass[3]);
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
}

void Map::drawTable(std::ifstream& ifile) {
    std::string str;
    int count = 0;

    while (std::getline(ifile, str)) {
        out(count + 1, 1, str);
        count++;
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
    GameObject* a;

    if (name == "player") {
        a = new Player(weight / 2, height / 2, 100, weight, height);
    } else if (name == "enemy") {
        a = new Enemy(parameters[0], parameters[1], 25, last_time, weight, height, 25);
    } else if (name == "altEnemy") {
        a = new AltEnemy(parameters[0], parameters[1], 50, last_time, weight, height, 50);
    } else if (name == "block") {
        a = new Block(parameters[0], parameters[1], 2000, parameters[2], now(), 5);
    } else if (name == "altBlock") {
        a = new AltBlock(parameters[0], parameters[1], 200, parameters[2], now(), 1);
    } if (name == "bullet") {
        if (direction == 'a') {
            a = new Bullet(parameters[0] - 2, parameters[1], last_time, direction);
        } else if (direction == 'd') {
            a = new Bullet(parameters[0] + 2, parameters[1], last_time, direction);
        } else if (direction == 'w') {
            a = new Bullet(parameters[0], parameters[1] - 2, last_time, direction);
        } else if (direction == 's') {
            a = new Bullet(parameters[0], parameters[1] + 2, last_time, direction);
        } else if (direction == 'j') {
            a = new Bullet(parameters[0] - 2, parameters[1] - 1, last_time, '7');
            objects.push_back(a);
            a = new Bullet(parameters[0] - 2, parameters[1], last_time, 'a');
            objects.push_back(a);
            a = new Bullet(parameters[0] - 2, parameters[1] + 1, last_time, '1');
        } else if (direction == 'l') {
            a = new Bullet(parameters[0] + 2, parameters[1] - 1, last_time, '9');
            objects.push_back(a);
            a = new Bullet(parameters[0] + 2, parameters[1], last_time, 'd');
            objects.push_back(a);
            a = new Bullet(parameters[0] + 2, parameters[1] + 1, last_time, '3');
        } else if (direction == 'i') {
            a = new Bullet(parameters[0] - 1, parameters[1] - 2, last_time, '7');
            objects.push_back(a);
            a = new Bullet(parameters[0], parameters[1] - 2, last_time, 'w');
            objects.push_back(a);
            a = new Bullet(parameters[0] + 1, parameters[1] - 2, last_time, '9');
        } else if (direction == 'k') {
            a = new Bullet(parameters[0] - 1, parameters[1] + 2, last_time, '1');
            objects.push_back(a);
            a = new Bullet(parameters[0], parameters[1] + 2, last_time, 's');
            objects.push_back(a);
            a = new Bullet(parameters[0] + 1, parameters[1] + 2, last_time, '3');
        }
    }

    objects.push_back(a);
}

int Map::countingOfRes(int& score) {
    int endOfGame = 0;

    for (auto & object : objects) {
        if (object->getHP() <= 0) {
            gamePoints += object->getGP();

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
