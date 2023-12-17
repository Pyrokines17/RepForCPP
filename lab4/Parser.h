#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <tuple>

template <typename... TupleT>
class CsvParser {
    std::tuple<TupleT...> arr;
    std::ifstream* input;
    int countOfSkip;

    template<typename TupleA, std::size_t... IdT>
    void readStr(TupleA& ta, const std::string& str, std::index_sequence<IdT...>) {
        std::stringstream line(str);
        std::string buf;

        ([&line, &ta, &buf] {
            std::getline(line, buf, ',');
            std::stringstream sBuf(buf);
            sBuf >> std::get<IdT>(ta);
        } (), ...);
    }

    template<typename TupleA, std::size_t TupleSize = std::tuple_size_v<TupleA>>
    void addData(TupleA& ta, const std::string& str) {
        readStr(ta, str, std::make_index_sequence<TupleSize>{});
    }

public:
    CsvParser(std::ifstream& instream, int countOfSkip);

    std::tuple<TupleT...> begin();
    std::tuple<TupleT...> end();
    std::tuple<TupleT...> next();

};

template <typename... TupleT>
CsvParser<TupleT...>::CsvParser(std::ifstream& instream, int countOfSkip) : countOfSkip(countOfSkip) {
    input = &instream;
}

template<typename... TupleT>
std::tuple<TupleT...> CsvParser<TupleT...>::begin() {
    std::string str;

    while (countOfSkip > 0) {
        getline(*input, str);
        countOfSkip--;
    }

    if (getline(*input, str)) {
        addData(arr, str);
        return arr;
    } else {
        std::tuple<TupleT...> clear;
        return clear;
    }
}

template<typename... TupleT>
std::tuple<TupleT...> CsvParser<TupleT...>::end() {
    std::tuple<TupleT...> clear;
    return clear;
}

template<typename... TupleT>
std::tuple<TupleT...> CsvParser<TupleT...>::next() {
    std::string str;

    if (getline(*input, str)) {
        addData(arr, str);
        return arr;
    } else {
        std::tuple<TupleT...> clear;
        return clear;
    }
}