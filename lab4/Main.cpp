#include "Tuple.h"
#include "TupleAdd.h"
#include "Parser.h"
#include "FlatMap.h"

int main() {
    std::cout << std::endl << "_____Task_№1_____" << std::endl;

    std::ofstream testFile("test.txt");
    std::tuple<int, std::string, double> t = {5, "abcd", 3.14};

    std::cout << t << std::endl;
    testFile << t << std::endl;

    MyTuple<int, std::string> testTuple(12, "qwe");
    std::cout << testTuple << std::endl;

    std::cout << "_________________" << std::endl;

    std::cout << std::endl << "_____Task_№2_____" << std::endl;

    //std::ifstream csv_stream("test.csv");
    std::ifstream csv_stream("test1.csv");
    //std::ifstream csv_stream("test2.csv");
    CsvParser<int, std::string> parser(csv_stream, 0);
    //CsvParser<int, std::string, double> parser(std::cin, 0);

    for (const auto& rs : parser) {
        std::cout << rs << std::endl;
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