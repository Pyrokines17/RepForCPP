#include "Blocks.h"

Block::Block(int weight, int height, int hp, int kit) :
        weight(weight), height(height), healthPoints(hp), gamePoints(5), kit(kit) {
}

char Block::action(int c, const std::vector<GameObject*>& objects) {
    if (c == 'e' and kit == 1 and
        objects[0]->getH() - 1 == height and objects[0]->getW() == weight) {
        objects[0]->HPChange(25);
        kit--;
    }

    return 0;
}

void Block::draw(const std::vector<int>& pairs, int c) { //ToDo animation
    static_cast<void>(c);
    attron(COLOR_PAIR(pairs[3]));

    out(height - 1, weight - 1, "/W\\");
    out(height, weight - 1, "<*>");
    out(height + 1, weight - 1, "\\M/");

    attroff(COLOR_PAIR(pairs[3]));
}