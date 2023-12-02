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
    // colors are COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
    // COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
    start_color();
    const int player_color_pair = 1;
    init_pair(player_color_pair, COLOR_BLUE, COLOR_BLACK);
    pairs.push_back(player_color_pair);
    const int enemy_color_pair = 2;
    init_pair(enemy_color_pair, COLOR_RED, COLOR_BLACK);
    pairs.push_back(enemy_color_pair);
    const int bullet_color_pair = 3;
    init_pair(bullet_color_pair, COLOR_RED, COLOR_BLACK);
    pairs.push_back(bullet_color_pair);
    const int block_color_pair = 4;
    init_pair(block_color_pair, COLOR_GREEN, COLOR_BLACK);
    pairs.push_back(block_color_pair);
    const int gun_color_pair = 5;
    init_pair(gun_color_pair, COLOR_YELLOW, COLOR_BLACK);
    pairs.push_back(gun_color_pair);
}

void out(int y, int x, const std::string& s) {
    wmove(stdscr, y, x);
    wprintw(stdscr, "%s", s.c_str());
}

steady_clock_t now() {
    return std::chrono::steady_clock::now();
}

void print(const char* str, int count, int height, int weight) {
    size_t title_size = snprintf(nullptr, 0, str, count);
    wmove(stdscr, height, (weight - static_cast<int>(title_size)) / 2);
    wprintw(stdscr, str, count);
}

void printFirstScr(int height, int centre) {
    std::string str = "How to play:";
    out(height / 2 - 2, centre, str);
    str = "{w, a, s, d} -- to move";
    out(height / 2 - 1, centre, str);
    str = "{<, ^, >, v} -- to shoot";
    out(height / 2, centre, str);
    str = "q -- to quit, e -- to use";
    out(height / 2 + 1, centre, str);
    str = "[ -- to save, ] -- to load";
    out(height / 2 + 2, centre, str);
}
