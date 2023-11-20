#include "SupFuncs.h"

void mainInit() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(50);
}

void initPairs(std::vector<int>& pairs) {
    // colors are COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
    start_color();
    const int main_color_pair = 1;
    init_pair(main_color_pair, COLOR_WHITE, COLOR_BLUE);
    pairs.push_back(main_color_pair);
    const int player_color_pair = 2;
    init_pair(player_color_pair, COLOR_WHITE, COLOR_RED);
    pairs.push_back(player_color_pair);
    const int bullet_color_pair = 3;
    init_pair(bullet_color_pair, COLOR_RED, COLOR_BLACK);
    pairs.push_back(bullet_color_pair);
    const int block_color_pair = 4;
    init_pair(block_color_pair, COLOR_GREEN, COLOR_BLACK);
    pairs.push_back(block_color_pair);
}

void out(int y, int x, const std::string& s) {
    wmove(stdscr, y, x);
    wprintw(stdscr, "%s", s.c_str());
}

steady_clock_t now() {
    return std::chrono::steady_clock::now();
}