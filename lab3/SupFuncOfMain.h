#pragma once

#include <curses.h>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;
using steady_clock_t = std::chrono::time_point<std::chrono::steady_clock>;

void initFunc();
void out(int y, int x, const std::string& s);
steady_clock_t now();