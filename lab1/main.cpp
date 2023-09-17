#include "Header.h"

int main(void) {
    FlatMap student1;

    FlatMap student2(student1);

    student1 = student1;
    student1 = student2;

    student1.size();

    student1["first_name"] = "Ivan";
    student1["last_name"] = "Petrov";
    student1["university"] = "NSU";
    student1["department"] = "FIT";
    student1["group"] = "...";

    std::cout << "Student: " << student1["first_name"] << " " << student1["last_name"] << "\n";
    std::cout << "Place of study: " << student1["university"] << " " << student1["department"] << "\n";

    student1.contains("first_name");
    student2.contains("first_name");

    student2.erase("first_name");
    student1.erase("first_name");

    student1.clear();

    student1.size();
    student1.contains("first_name");

    return 0;
}