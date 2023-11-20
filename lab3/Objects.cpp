#include "Objects.h"

Bullet::Bullet(int w, int h, steady_clock_t last_time, char direction) : w(w), h(h), last_time(last_time), direction(direction) {
}

char Bullet::action(int c, std::vector<GameObject*>& objects) { //ToDo find enemy's
    static_cast<void>(c);
    int i = 0,
        allow = 0,
        id;

    for (auto & object : objects){
        if (object != this) {
            if (w == object->getW() and h == object->getH()){
                out(h, w, "#");
                allow = 1;
            }
        } else {
           id = i;
        }
        i++;
    }

    if (allow == 1) { //(?)//
        objects.erase(objects.begin() + id);
    }

    return 0;
}

void Bullet::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[2]));
    if ((now() - last_time) / 1ms > 300) {
        if (direction == 'w') {
            h--;
        } else if (direction == 's') {
            h++;
        } else if (direction == 'd') {
            w++;
        } else if (direction == 'a') {
            w--;
        }
        last_time = now();
    }
    out(h, w, "*");
    attroff(COLOR_PAIR(pairs[2]));
}

Player::Player(int w, int h, int w1, int h1) : w(w), h(h), w1(w1), h1(h1) {
}

char Player::action(int c, std::vector<GameObject*>& objects) {
    char res = 0;

    for (auto & object : objects){
        if (object != this){
            if (w == object->getW() and h == object->getH()){
                out(h, w, "#");
            }
        }
    }

    switch(c) {
        case 'a': {
            int allow;
            for (auto & object : objects) {
                if (object->getH() == h and object->getW() == w - 1) {
                    allow = 0;
                    break;
                }
                allow = 1;
            }
            if (allow == 1) {
                w--;
            }
            break;
        }
        case 'd': {
            int allow;
            for (auto & object : objects) {
                if (object->getH() == h and object->getW() == w + 1) {
                    allow = 0;
                    break;
                }
                allow = 1;
            }
            if (allow == 1) {
                w++;
            }
            break;
        }
        case 'w': {
            int allow;
            for (auto & object : objects) {
                if (object->getH() == h - 1 and object->getW() == w) {
                    allow = 0;
                    break;
                }
                allow = 1;
            }
            if (allow == 1) {
                h--;
            }
            break;
        }
        case 's': {
            int allow;
            for (auto & object : objects) {
                if (object->getH() == h + 1 and object->getW() == w) {
                    allow = 0;
                    break;
                }
                allow = 1;
            }
            if (allow == 1) {
                h++;
            }
            break;
        }
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
    w = std::clamp(w, 1, w1-2);
    h = std::clamp(h, 1, h1-2);
    out(h, w, "P");
    attroff(COLOR_PAIR(pairs[0]));
}

Enemy::Enemy(int w, int h, steady_clock_t last_time, int w1, int h1) : w(w), h(h), w1(w1), h1(h1), last_time(last_time) {
}

char Enemy::action(int c, std::vector<GameObject*>& objects) {
    static_cast<void>(c);

    if ((now() - last_time) / 1ms > 300) {
        int wp = objects[0]->getW();
        int hp = objects[0]->getH();

        if (hp + 7 < h) {
            h--;
        } else if (hp - 7 > h) {
            h++;
        } else if (wp - 10 > w) {
            w++;
        } else if (wp + 10 < w) {
            w--;
        }
        last_time = now();
    }

    return 0;
}

void Enemy::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[1]));
    w = std::clamp(w, 1, w1-2);
    h = std::clamp(h, 1, h1-2);
    out(h, w, "E");
    attroff(COLOR_PAIR(pairs[1]));
}

Block::Block(int w, int h) : w(w), h(h) {
}

char Block::action(int c, std::vector<GameObject*>& objects) {
    static_cast<void>(c);
    for (auto & object : objects){
        if (object != this){
            if (w == object->getW() and h == object->getH()){
                out(h, w, "#");
            }
        }
    }
    return 0;
}

void Block::draw(const std::vector<int>& pairs) {
    attron(COLOR_PAIR(pairs[3]));
    out(h, w, "B");
    attroff(COLOR_PAIR(pairs[3]));
}

Map::Map(int w, int h, int countOfEnemy, int countOfBlock) : w(w), h(h), countOfEnemy(countOfEnemy), countOfBlock(countOfBlock) {
}

void Map::drawBorders() const {
    for (int i = 1; i < w; ++i) {
        out(0, i, "-");
        out(h-1, i, "-");
    }

    for (int i = 1; i < h-1; ++i) {
        out(i, 0, "|");
        out(i, w-1, "|");
    }

    out(0, 0, "+");
    out(0, w-1, "+");
    out(h-1, 0, "+");
    out(h-1, w-1, "+");
}

void Map::printStat() const {
    const char* title_format_str = "Enemy's: %lu";

    size_t title_size = snprintf(nullptr, 0, title_format_str, countOfEnemy);
    wmove(stdscr, 1, (w - static_cast<int>(title_size)) / 2);
    wprintw(stdscr, title_format_str, countOfEnemy);

    const char* title_format_str1 = "Bullet's: %lu";

    size_t title_size1 = snprintf(nullptr, 0, title_format_str1, objects.size() - countOfEnemy - countOfBlock - 1);
    wmove(stdscr, 2, (w - static_cast<int>(title_size1)) / 2);
    wprintw(stdscr, title_format_str1, objects.size() - countOfEnemy - countOfBlock - 1);
}

void Map::actObj(int c) {
    for (auto & object : objects){
        char res = object->action(c, objects);

        if (res) {
            initBul(object->getW(), object->getH(), now(), res);
        }
    }

    if (!buf.empty()) {
        for (auto & object : buf) {
            objects.push_back(object);
        }
        buf.clear();
    }
}

void Map::drawObj(const std::vector<int>& pairs) {
    for (auto & object : objects){
        object->draw(pairs);
    }
}

//ToDo remake init's
void Map::initPlayer() {
    auto* a = new Player(w/2, h/2, w, h);
    objects.push_back(a);
}

void Map::initEnemy(int w1, int h1, steady_clock_t last_time, int w2, int h2) {
    auto* a = new Enemy(w1, h1, last_time, w2, h2);
    objects.push_back(a);
}

void Map::initBlock(int w1, int h1) {
    auto* a = new Block(w1, h1);
    objects.push_back(a);
}

void Map::initBul(int w1, int h1, steady_clock_t last_time, char direction) {
    GameObject* a;
    if (direction == 'a') {
        a = new Bullet(w1-1, h1, last_time, direction);
    } else if (direction == 'd') {
        a = new Bullet(w1+1, h1, last_time, direction);
    } else if (direction == 'w') {
        a = new Bullet(w1, h1-1, last_time, direction);
    } else {
        a = new Bullet(w1, h1+1, last_time, direction);
    }
    buf.push_back(a);
}

void Map::erase() {
    objects.erase(
            std::remove_if(objects.begin(), objects.end(), [this](GameObject *b) { return b->getH() < 1 or b->getW() < 1 or b->getH() > this->h - 2 or b->getW() > this->w - 2; }),
            objects.end()
    );
}
