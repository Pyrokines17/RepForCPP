#include "Tuple.h"
#include "Parser.h"
#include "FlatMap.h"

int main() {
    std::cout << std::endl << "_____Task_№1_____" << std::endl;

    std::tuple<int, std::string, double> t = {5, "abcd", 3.14};
    std::cout << t << std::endl;

    std::cout << "_________________" << std::endl;

    std::cout << std::endl << "_____Task_№2_____" << std::endl;

    std::ifstream csv_stream("test.csv");
    CsvParser<int, std::string, double> parser(std::cin, 1 /*skip first lines count*/);
    for (std::tuple<int, std::string, double> rs : parser) {
        std::cout << rs << "\n";
    }

    std::cout << "_________________" << std::endl;

    std::cout << std::endl << "_____Task_№3_____" << std::endl;

    FlatMap<std::string, std::string> map1;
    FlatMap<std::string, double> map2;
    FlatMap<int, std::string> map3;

    map1["First_name"] = "Ivan";
    map1["Last_name"] = "Ivanov";

    for (auto & it : map1) {
        std::cout << it.key << ": " << it.value << std::endl;
    }

    std::cout << "_________________" << std::endl;

    map2["pi"] = 3.14;
    map2["e"] = 2.71;

    for (auto & it : map2) {
        std::cout << it.key << ": " << it.value << std::endl;
    }
    std::cout << "_________________" << std::endl;

    map3[1] = "one";
    map3[10] = "ten";
    map3[5] = "five";

    for (auto & it : map3) {
        std::cout << it.key << ": " << it.value << std::endl;
    }

    std::cout << "_________________" << std::endl;

    return 0;
}