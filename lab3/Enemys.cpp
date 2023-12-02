#include "Enemys.h"

Enemy::Enemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp, int cob) :
GameObject(weight, height, hp, gp), weightOfBorder(weightOfBorder), heightOfBorder(heightOfBorder), last_time(last_time), countOfBullets(cob), reload(0) {
    body.emplace_back("{?}");
    body.emplace_back("!|\\");
    body.emplace_back("/ |");

    parts.emplace_back("/|!");
    parts.emplace_back("| \\");
}

int Enemy::checkStep(const std::vector<GameObject*>& objects, int futWeight, int futHeight) {
    for (auto & object : objects) {
        if (object->getH() == futHeight and object->getW() == futWeight) {
            return 0;
        }
    }

    return 1;
}

int Enemy::look(const std::vector<GameObject *> &objects, char side) {
    int id = 0,
        distance = 999;
    switch (side) {
        case 'a': for (auto & object : objects) {
            if (object->getW() < weight and object->getH() >= height - 1 and object->getH() <= height + 1) {
                if (weight - object->getW() < distance) {
                    distance = weight - object->getW();
                    id = object->getGP();
                }
            }
        } break;
        case 'd': for (auto & object : objects) {
            if (object->getW() > weight and object->getH() >= height - 1 and object->getH() <= height + 1) {
                if (object->getW() - weight < distance) {
                    distance = object->getW() - weight;
                    id = object->getGP();
                }
            }
        } break;
        case 'w': for (auto & object : objects) {
            if (object->getH() < height and object->getW() >= weight - 1 and object->getW() <= weight + 1) {
                if (height - object->getH() < distance) {
                    distance = height - object->getH();
                    id = object->getGP();
                }
            }
        } break;
        case 's': for (auto & object : objects) {
            if (object->getH() > height and object->getW() >= weight - 1 and object->getW() <= weight + 1) {
                if (object->getH() - height < distance) {
                    distance = object->getH() - height;
                    id = object->getGP();
                }
            }
        } break;
        default: break;
    }

    return id;
}

char Enemy::action(int c, const std::vector<GameObject*>& objects) {
    static_cast<void>(c);
    char res = 0;

    if ((now() - last_time) / 1ms > 300) {
        int wt = objects[0]->getW(),
            ht = objects[0]->getH();
        int step = 0;

        if (ht + 7 < height) {
            if (checkStep(objects, weight, height - 1)) {
                height--;
                step = 1;
            }
        } else if (ht - 7 > height) {
            if (checkStep(objects, weight, height + 1)) {
                height++;
                step = 1;
            }
        } else if (wt - 10 > weight) {
            if (checkStep(objects, weight + 1, height)) {
                weight++;
                step = 1;
            }
        } else if (wt + 10 < weight) {
            if (checkStep(objects, weight - 1, height)) {
                weight--;
                step = 1;
            }
        }

        if (step == 0) {
            if (wt < weight and
                ((ht > height and weight - wt < ht - height) or
                 (ht < height and weight - wt < height - ht))) {
                weight--;
            } else if (wt > weight and
                       ((ht > height and wt - weight < ht - height) or
                        (ht < height and wt - weight < height - ht))) {
                weight++;
            } else if (ht > height and
                       ((wt > weight and wt - weight > ht - height) or
                        (wt < weight and weight - wt > ht - height))) {
                height++;
            } else if (ht < height and
                       ((wt > weight and wt - weight > height - ht) or
                        (wt < weight and weight - wt > height - ht))) {
                height--;
            }
        }

        int subOfW = wt > weight ? wt - weight : weight - wt,
            subOfH = ht > height ? ht - height : height - ht;

        if (reload == 0 and subOfW <= weightOfBorder / 3 and subOfH <= heightOfBorder / 2) {
            if (height <= ht + 2 and height >= ht - 2 and weight > wt and
            look(objects, 'a') != 25 and look(objects, 'a') != 50) {
                res = 'a';
                countOfBullets--;
            } else if (height <= ht + 2 and height >= ht - 2 and weight < wt and
            look(objects, 'd') != 25 and look(objects, 'd') != 50) {
                res = 'd';
                countOfBullets--;
            } else if (weight >= wt - 2 and weight <= wt + 2 and height > ht and
            look(objects, 'w') != 25 and look(objects, 'w') != 50) {
                res = 'w';
                countOfBullets--;
            } else if (weight >= wt - 2 and weight <= wt + 2 and height < ht and
            look(objects, 's') != 25 and look(objects, 's') != 50) {
                res = 's';
                countOfBullets--;
            }

            if (countOfBullets == 0) {
                reload = 1;
            }
        }

        if (reload == 1) {
            countOfBullets++;

            if (countOfBullets == 6) {
                reload = 0;
            }
        }
    }

    return res;
}

void Enemy::draw(const std::vector<int>& pairs, int c) {
    static_cast<void>(c);
    attron(COLOR_PAIR(pairs[1]));

    weight = std::clamp(weight, 2, weightOfBorder - 3);
    height = std::clamp(height, 1, heightOfBorder - 3);

    if ((now() - last_time) / 1ms > 300) {
        std::swap(body[1], parts[0]);
        std::swap(body[2], parts[1]);
        last_time = now();
    }

    out(height - 1, weight - 1, body[0]);
    out(height, weight - 1, body[1]);
    out(height + 1, weight - 1, body[2]);

    attroff(COLOR_PAIR(pairs[1]));
}

AltEnemy::AltEnemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp, int cob) :
Enemy(weight, height, hp, last_time, weightOfBorder, heightOfBorder, gp, cob) {
    body.clear();
    parts.clear();

    body.emplace_back("{%}");
    body.emplace_back("?0\\");
    body.emplace_back("/ |");

    parts.emplace_back("/0?");
    parts.emplace_back("| \\");
}

char AltEnemy::action(int c, const std::vector<GameObject *> &objects) {
    static_cast<void>(c);
    char res = 0;

    if ((now() - last_time) / 1ms > 600) {
        int wt = objects[0]->getW(),
            ht = objects[0]->getH();
        int step = 0;

        if (ht + 7 < height) {
            if (checkStep(objects, weight, height - 1)) {
                height--;
                step = 1;
            }
        } else if (ht - 7 > height) {
            if (checkStep(objects, weight, height + 1)) {
                height++;
                step = 1;
            }
        } else if (wt - 10 > weight) {
            if (checkStep(objects, weight + 1, height)) {
                weight++;
                step = 1;
            }
        } else if (wt + 10 < weight) {
            if (checkStep(objects, weight - 1, height)) {
                weight--;
                step = 1;
            }
        }

        if (step == 0) {
            if (wt < weight and
                ((ht > height and weight - wt < ht - height) or
                 (ht < height and weight - wt < height - ht))) {
                weight--;
            } else if (wt > weight and
                       ((ht > height and wt - weight < ht - height) or
                        (ht < height and wt - weight < height - ht))) {
                weight++;
            } else if (ht > height and
                       ((wt > weight and wt - weight > ht - height) or
                        (wt < weight and weight - wt > ht - height))) {
                height++;
            } else if (ht < height and
                       ((wt > weight and wt - weight > height - ht) or
                        (wt < weight and weight - wt > height - ht))) {
                height--;
            }
        }

        int subOfW = wt > weight ? wt - weight : weight - wt,
            subOfH = ht > height ? ht - height : height - ht;

        if (reload == 0 and subOfH <= heightOfBorder / 2 and subOfW <= weightOfBorder / 3) {
            if (height <= ht + 2 and height >= ht - 2 and weight > wt and
            look(objects, 'a') != 25 and look(objects, 'a') != 50) {
                res = 'j';
                countOfBullets--;
            } else if (height <= ht + 2 and height >= ht - 2 and weight < wt and
            look(objects, 'd') != 25 and look(objects, 'd') != 50) {
                res = 'l';
                countOfBullets--;
            } else if (weight >= wt - 2 and weight <= wt + 2 and height > ht and
            look(objects, 'w') != 25 and look(objects, 'w') != 50) {
                res = 'i';
                countOfBullets--;
            } else if (weight >= wt - 2 and weight <= wt + 2 and height < ht and
            look(objects, 's') != 25 and look(objects, 's') != 50) {
                res = 'k';
                countOfBullets--;
            }

            if (countOfBullets == 0) {
                reload = 1;
            }
        }

        if (reload == 1) {
            countOfBullets++;

            if (countOfBullets == 2) {
                reload = 0;
            }
        }
    }

    return res;
}

void AltEnemy::draw(const std::vector<int> &pairs, int c) {
    static_cast<void>(c);
    attron(COLOR_PAIR(pairs[1]));

    weight = std::clamp(weight, 2, weightOfBorder - 3);
    height = std::clamp(height, 1, heightOfBorder - 3);

    if ((now() - last_time) / 1ms > 600) {
        std::swap(body[1], parts[0]);
        std::swap(body[2], parts[1]);
        last_time = now();
    }

    out(height - 1, weight - 1, body[0]);
    out(height, weight - 1, body[1]);
    out(height + 1, weight - 1, body[2]);

    attroff(COLOR_PAIR(pairs[1]));
}
