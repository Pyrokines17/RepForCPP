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

    void updateData() {
        std::string str;

        if (std::getline(*input, str)) {
            addData(arr, str);
        } else {
            arr = std::tuple<TupleT...>();
        }
    }

public:
    class Iterator {
    public:
        using iteratorCategory [[maybe_unused]] = std::input_iterator_tag;
        using differenceType [[maybe_unused]] = std::ptrdiff_t;
        using valueType [[maybe_unused]] = std::tuple<TupleT...>;
        using pointer = std::tuple<TupleT...>*;
        using reference = std::tuple<TupleT...>&;

        Iterator(pointer ptr, CsvParser<TupleT...>& prs) : m_ptr(ptr) {
            pars = &prs;
            buf = *ptr;
        };

        reference operator * () const { return *m_ptr; }
        pointer operator -> () { return m_ptr; }

        Iterator& operator ++ () {
            pars->updateData();
            buf = pars->arr;
            return *this;
        }
        Iterator operator ++ (int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator == (const Iterator& a, const Iterator& b) {
            return a.buf == b.buf;
        };
        friend bool operator != (const Iterator& a, const Iterator& b) {
            return a.buf != b.buf;
        };

        private:
            pointer m_ptr;
            std::tuple<TupleT...> buf;
            CsvParser<TupleT...>* pars;

        };

    CsvParser(std::ifstream& instream, int countOfSkip);

    Iterator begin();
    Iterator end();

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

    return ++Iterator(&arr, *this);
}

template<typename... TupleT>
typename CsvParser<TupleT...>::Iterator CsvParser<TupleT...>::end() {
    std::tuple<TupleT...> clear;
    return Iterator(&clear, *this);
}
