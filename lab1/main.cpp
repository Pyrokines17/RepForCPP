#include "header.h"
#include <iostream>

int main(void) {
    FlatMap student1;

    FlatMap student2(student1);

    student1 = student2;

    student1.size();

    return 0;
}