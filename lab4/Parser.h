#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <tuple>
#include <iterator>
#include <cstddef>

template <typename... TupleT>
class CsvParser {
    static std::tuple<TupleT...> arr;
    static std::ifstream* input;
    int countOfSkip;

    template<typename TupleA, std::size_t... IdT>
    static void readStr(TupleA& ta, const std::string& str, std::index_sequence<IdT...>) {
        std::stringstream line(str);
        std::string buf;

        ([&line, &ta, &buf] {
            std::getline(line, buf, ',');
            std::stringstream sBuf(buf);
            sBuf >> std::get<IdT>(ta);
        } (), ...);
    }

    template<typename TupleA, std::size_t TupleSize = std::tuple_size_v<TupleA>>
    static void addData(TupleA& ta, const std::string& str) {
        readStr(ta, str, std::make_index_sequence<TupleSize>{});
    }

public:
    struct Iterator {
        using iteratorCategory = std::input_iterator_tag;
        using differenceType = std::ptrdiff_t;
        using valueType = std::tuple<TupleT...>;
        using pointer = std::tuple<TupleT...>*;
        using reference = std::tuple<TupleT...>&;

        explicit Iterator(pointer ptr) : m_ptr(ptr) {};

        reference operator * () const { return &m_ptr; }
        pointer operator -> () { return m_ptr; }

        Iterator operator ++ () {
            std::string str;

            if (getline(*input, str)) {
                addData(arr, str);
            } else {
                std::tuple<TupleT...> clear;
                arr = clear;
            }

            return Iterator(&arr);
        }

        Iterator operator ++ (int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator == (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator != (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;

        };

    CsvParser(std::ifstream& instream, int countOfSkip);

    Iterator begin();
    Iterator end();

    //std::tuple<TupleT...> next();

};

template <typename... TupleT>
CsvParser<TupleT...>::CsvParser(std::ifstream& instream, int countOfSkip) : countOfSkip(countOfSkip) {
    input = &instream;
}

template<typename... TupleT>
typename CsvParser<TupleT...>::Iterator CsvParser<TupleT...>::begin() {
    std::string str;

    while (countOfSkip > 0) {
        getline(*input, str);
        countOfSkip--;
    }

    if (getline(*input, str)) {
        addData(arr, str);
    } else {
        std::tuple<TupleT...> clear;
        arr = clear;
    }

    return Iterator(&arr);
}

template<typename... TupleT>
typename CsvParser<TupleT...>::Iterator CsvParser<TupleT...>::end() {
    std::tuple<TupleT...> clear;
    return Iterator(&clear);
}

//template<typename... TupleT>
//std::tuple<TupleT...> CsvParser<TupleT...>::next() {
//    std::string str;
//
//    if (getline(*input, str)) {
//        addData(arr, str);
//        return arr;
//    } else {
//        std::tuple<TupleT...> clear;
//        return clear;
//    }
//}
