#include "task1.h"
#include "task2.h"
#include <iostream>

int main() {
    std::cout << say::say_hello("world") << "\n";

    const int x = 2;
    const int y = 3;

    std::cout << x << " + " << y << " = " << sum::plus(x, y) << "\n";

    return 0;
}
