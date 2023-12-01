#include "Enemys.h"

Enemy::Enemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp) :
GameObject(weight, height, hp, gp), weightOfBorder(weightOfBorder), heightOfBorder(heightOfBorder), anim(0), last_time(last_time) {
    body.emplace_back("{?}");
    body.emplace_back("/|\\");
    body.emplace_back("/ \\");

    parts.emplace_back("!|\\");
    parts.emplace_back("/ |");
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

char Enemy::action(int c, const std::vector<GameObject*>& objects) { //ToDo method look() and reload()
    static_cast<void>(c);
    char res = 0;

    if ((now() - last_time) / 1ms > 300) {
        int wt = objects[0]->getW(),
            ht = objects[0]->getH();
        int step = 0,
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

        if (height <= ht + 2 and height >= ht - 2 and weight > wt) {
            res = 'a';
            shoot = 1;
        }
        if (height <= ht + 2 and height >= ht - 2 and weight < wt and shoot == 0) {
            res = 'd';
            shoot = 1;
        }
        if (weight >= wt - 2 and weight <= wt + 2 and height > ht and shoot == 0) {
            res = 'w';
            shoot = 1;
        }
        if (weight >= wt - 2 and weight <= wt + 2 and height < ht and shoot == 0) {
            res = 's';
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
        if (anim == 0) {
            body[1] = parts[0];
            body[2] = parts[1];
            anim = 1;
        } else {
            body[1] = parts[2];
            body[2] = parts[3];
            anim = 0;
        }
        last_time = now();
    }

    out(height - 1, weight - 1, body[0]);
    out(height, weight - 1, body[1]);
    out(height + 1, weight - 1, body[2]);

    attroff(COLOR_PAIR(pairs[1]));
}

AltEnemy::AltEnemy(int weight, int height, int hp, steady_clock_t last_time, int weightOfBorder, int heightOfBorder, int gp) :
Enemy(weight, height, hp, last_time, weightOfBorder, heightOfBorder, gp) {
    body.clear();
    parts.clear();

    body.emplace_back("{%}");
    body.emplace_back("/0\\");
    body.emplace_back("/ \\");

    parts.emplace_back("?0\\");
    parts.emplace_back("/ |");
    parts.emplace_back("/0?");
    parts.emplace_back("| \\");
}

char AltEnemy::action(int c, const std::vector<GameObject *> &objects) {
    static_cast<void>(c);
    char res = 0;

    if ((now() - last_time) / 1ms > 600) {
        int wt = objects[0]->getW(),
            ht = objects[0]->getH();
        int step = 0,
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

        if (height <= ht + 2 and height >= ht - 2 and weight > wt) {
            res = 'j';
            shoot = 1;
        }
        if (height <= ht + 2 and height >= ht - 2 and weight < wt and shoot == 0) {
            res = 'l';
            shoot = 1;
        }
        if (weight >= wt - 2 and weight <= wt + 2 and height > ht and shoot == 0) {
            res = 'i';
            shoot = 1;
        }
        if (weight >= wt - 2 and weight <= wt + 2 and height < ht and shoot == 0) {
            res = 'k';
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
        if (anim == 0) {
            body[1] = parts[0];
            body[2] = parts[1];
            anim = 1;
        } else {
            body[1] = parts[2];
            body[2] = parts[3];
            anim = 0;
        }
        last_time = now();
    }

    out(height - 1, weight - 1, body[0]);
    out(height, weight - 1, body[1]);
    out(height + 1, weight - 1, body[2]);

    attroff(COLOR_PAIR(pairs[1]));
}
