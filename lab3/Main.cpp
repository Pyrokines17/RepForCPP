#include "Game.h"

int main() {
    mainInit();

    std::random_device rd;
    std::mt19937 gen(rd());

    if (has_colors() == FALSE) {
        endwin();
        std::cerr << "Your terminal does not support color\n";
        exit(1);
    }

    std::vector<int> pairs;
    initPairs(pairs);

    int h,
        w;
    getmaxyx(stdscr, h, w);

    std::uniform_int_distribution<> dist1(1, h - 2);
    std::uniform_int_distribution<> dist2(1, w - 2);
    std::uniform_int_distribution<> dist3(1, 25);

    int count = dist3(gen);
    int count1 = dist3(gen);
    Map map(w, h, count, count1);
    map.initPlayer();

    for (int i = 0; i < count; i++){
        map.initEnemy(dist2(gen), dist1(gen), now(), w, h);
    }

    for (int i = 0; i < count1; i++){
        map.initBlock(dist2(gen), dist1(gen));
    }

    int c;
    while ('q' != (c = getch())) {
        clear();
        map.drawBorders();
        map.printStat();

        map.actObj(c);
        map.drawObj(pairs);

        map.erase();
        refresh();
    }

    endwin();

    return 0;
}