#include <iostream>
#include <curses.h>
#include <chrono>
#include <vector>

using namespace std::chrono_literals;
using steady_clock_t = std::chrono::time_point<std::chrono::steady_clock>;

void mainInit();
void initPairs(std::vector<int>& pairs);
void out(int y, int x, const std::string& s);
steady_clock_t now();