#include "Game.h"

int main() {
    mainInit();

    std::random_device rd;
    std::mt19937 gen(rd());

    if (has_colors() == FALSE) {
        endwin();
        std::cerr << "Your terminal does not support color :( \n";
        exit(1);
    }

    std::vector<int> pairs;
    initPairs(pairs);

    int height,
        weight;
    getmaxyx(stdscr, height, weight);

    std::uniform_int_distribution<> distH(2, height - 3);
    std::uniform_int_distribution<> distW(2, weight - 3);
    std::uniform_int_distribution<> distC(1, 25);
    std::uniform_int_distribution<> distK(0, 1);

    int countOfEnemy = distC(gen);
    int countOfBlock = distC(gen);
    Map map(weight, height, countOfEnemy, countOfBlock);
    std::vector<int> parameters = {0, 0, 0};

    map.init(parameters, now(), '-', "player");
    int score = 0;

    for (int i = 0; i < countOfEnemy; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        map.init(parameters, now(), '-', "enemy");
    }

    for (int i = 0; i < countOfBlock; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        parameters[2] = distK(gen);
        map.init(parameters, now(), '-', "block");
    }

    std::ofstream out;
    out.open("table.txt");
    std::ifstream in;
    in.open("table.txt");

    char b;
    while ('q' != (b = getch())) {
        clear();
        printFirstScr(height, weight);
        refresh();
    }

    int c,
        res = 0;
    while ('q' != (c = getch())) {
        clear();
        map.drawBorders();

        map.actionOfObj(c);
        map.drawObj(pairs, c);

        map.printStat(score);

        res = map.countingOfRes(score);

        if (res > 0) {
            break;
        }

        map.erase();
        refresh();
    }

    const char* str;
    if (res == 1) {
        str = "You win! Your score: %d";
    } else if (res == 2) {
        str = "You lose... Your score: %d";
    } else if (res == 0) {
        str = "You leave :( Your score: %d";
    }

    while ('q' != (c = getch())) {
        clear();
        print(str, score, height / 2, weight);
        refresh();
    }

    endwin();

    return 0;
}