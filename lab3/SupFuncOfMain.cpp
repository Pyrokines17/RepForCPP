#include "SupFuncOfMain.h"

void initFunc() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(50);
}

void out(int y, int x, const std::string& s) {
    wmove(stdscr, y, x);
    wprintw(stdscr, "%s", s.c_str());
}

steady_clock_t now() {
    return std::chrono::steady_clock::now();
}