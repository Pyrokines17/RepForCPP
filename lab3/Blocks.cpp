#include "Blocks.h"

Block::Block(int weight, int height, int hp, int kit, steady_clock_t last_time, int gp) :
GameObject(weight, height, hp, gp), kit(kit), last_time(last_time) {
    parts.emplace_back("/W\\");
    parts.emplace_back("<*>");
    parts.emplace_back("\\M/");
}

char Block::action(int c, const std::vector<GameObject*>& objects) {
    int subW = objects[0]->getW() - weight;
    int subH = objects[0]->getH() - height;

    if (c == 'e' && kit == 1 && (subW * subW + subH * subH <= 2)) {
        objects[0]->HPChange(25);
        kit--;
    }

    return 0;
}

void Block::draw(const std::vector<int>& pairs, int c) {
    static_cast<void>(c);
    attron(COLOR_PAIR(pairs[3]));

    if ((now() - last_time) / 1ms > 600) {
        std::swap(parts[0], parts[2]);
        last_time = now();
    }

    out(height - 1, weight - 1, parts[0]);
    out(height, weight - 1, parts[1]);
    out(height + 1, weight - 1, parts[2]);

    attroff(COLOR_PAIR(pairs[3]));
}

AltBlock::AltBlock(int weight, int height, int hp, int kit, steady_clock_t last_time, int gp) :
Block(weight, height, hp, kit, last_time, gp) {
    parts.clear();

    parts.emplace_back("f~q");
    parts.emplace_back("|$|");
    parts.emplace_back("t-g");
}