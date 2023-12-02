#pragma once

#include <iostream>
#include <curses.h>
#include <chrono>
#include <vector>

#include "SFML/Audio.hpp"

using namespace std::chrono_literals;
using steady_clock_t = std::chrono::time_point<std::chrono::steady_clock>;

void mainInit();
void initPairs(std::vector<int>& pairs);

void out(int y, int x, const std::string& s);
steady_clock_t now();

void print(const char* str, int count, int height, int weight);
void printFirstScr(int height, int centre);
