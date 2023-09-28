#include "FlatMap.h"

int main(void) {
    FlatMap student1;

    student1.size();

    student1["first_name"] = "Ivan";
    student1["last_name"] = "Petrov";
    student1["university"] = "NSU";
    student1["department"] = "FIT";
    student1["group"] = "...";

    student1.size();

    std::cout << "Student: " << student1["first_name"] << " " << student1["last_name"] << "\n";
    std::cout << "Place of study: " << student1["university"] << " " << student1["department"] << "\n";

    student1.find("group");
    student1.find("...");

    FlatMap student2(student1);

    for (auto it = student2.begin(); it != student2.end(); ++it) {
        std::cout << it->key << ": " << it->value << "\n";
    }

    student2.contains("first_name");
    student2.erase("first_name");
    student2.contains("first_name");
    student2.erase("first_name");

    student2 = student2;
    student2 = student1;    

    student1.clear();

    student1.size();
    student1.contains("first_name");

    FlatMap student3(std::move(student1));

    student3 = std::move(student2);

    return 0;
}
