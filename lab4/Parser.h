#pragma once

#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <iterator>
#include <vector>

struct errWithRead : public std::exception {
    explicit errWithRead(int count, const std::vector<int>& place) : numberOfStr(count), numberOfPlace(place) {
    };

    [[nodiscard]] std::string getErrorPlace() const {
        std::string str = " in string № ";
        std::string numStr = std::to_string(numberOfStr);
        str += numStr + " (";

        for (int i : numberOfPlace) {
            std::string numPl = std::to_string(i);
            str += numPl + ",";
        }

        str.pop_back();
        str += ")";

        return str;
    }

private:
    int numberOfStr;
    std::vector<int> numberOfPlace;

};

template <typename... TupleT>
class CsvParser {
    std::tuple<TupleT...> arr;
    std::vector<std::string> rest;
    std::ifstream* input0;
    std::istream* input1;
    int countOfSkip;
    int countOfRead;

    template<typename TupleA, std::size_t... IdT>
    void readStr(TupleA& ta, const std::string& str, std::index_sequence<IdT...>) {
        std::stringstream line(str);
        std::string buf;
        std::vector<int> place;
        int err = 0;

        ([&line, &ta, &buf, &err, &place] {
            if (!std::getline(line, buf, ',') || buf.empty()) {
                buf = "_";
                err = 1;
                place.push_back(IdT + 1);
            }
            std::stringstream sBuf(buf);
            if (!(sBuf >> std::get<IdT>(ta)) && err == 0) {
                err = 1;
                place.push_back(IdT + 1);
            }
        } (), ...);

        while (std::getline(line, buf, ',')) {
            rest.push_back(buf);
        }

        if (err == 1) {
            throw errWithRead(countOfRead, place);
        }
    }

    template<typename TupleA, std::size_t TupleSize = std::tuple_size_v<TupleA>>
    void addData(TupleA& ta, const std::string& str) {
        try {
            readStr(ta, str, std::make_index_sequence<TupleSize>{});
        } catch (const errWithRead& ex) {
            std::cerr << std::endl << "Error type while reading" << ex.getErrorPlace() << std::endl;
        }
    }

    void updateData() {
        rest.clear();
        std::string str;

        if (input1 == nullptr) {
            if (std::getline(*input0, str)) {
                ++countOfRead;
                addData(arr, str);
            } else {
                arr = std::tuple<TupleT...>();
            }
        } else {
            std::getline(*input1, str);
            if (!str.empty()) {
                ++countOfRead;
                addData(arr, str);
            } else {
                arr = std::tuple<TupleT...>();
            }
        }
    }

public:
    class Iterator {
    public:
        using iteratorCategory [[maybe_unused]] = std::input_iterator_tag;
        using differenceType [[maybe_unused]] = std::ptrdiff_t;
        using valueType = std::tuple<TupleT...>;
        using pointer = valueType*;
        using reference = valueType&;

        Iterator(pointer ptr, CsvParser<TupleT...>& prs, std::vector<std::string>& restOfIer) : m_ptr(ptr), restOfIter(restOfIer) {
            pars = &prs;
            buf = *ptr;
        };

        reference operator * () const {
            return *m_ptr;
        }
        pointer operator -> () {
            return m_ptr;
        }

        Iterator& operator ++ () {
            pars->updateData();
            buf = pars->arr;
            restOfIter = pars->getRest();
            return *this;
        }
        Iterator operator ++ (int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator == (const Iterator& a, const Iterator& b) {
            return (a.buf == b.buf && a.restOfIter == b.restOfIter);
        };
        friend bool operator != (const Iterator& a, const Iterator& b) {
            return (a.buf != b.buf || a.restOfIter != b.restOfIter);
        };

        private:
            pointer m_ptr;
            valueType buf;
            CsvParser<TupleT...>* pars;
            std::vector<std::string> restOfIter;

        };

    CsvParser(std::ifstream& instream, int countOfSkip);
    CsvParser(std::istream& instream, int countOfSkip);

    std::vector<std::string> getRest() {return rest;};

    Iterator begin();
    Iterator end();

};

template <typename... TupleT>
CsvParser<TupleT...>::CsvParser(std::ifstream& instream, int countOfSkip) : countOfSkip(countOfSkip), countOfRead(0) {
    input0 = &instream;
    input1 = nullptr;
}

template <typename... TupleT>
CsvParser<TupleT...>::CsvParser(std::istream& instream, int countOfSkip) : countOfSkip(countOfSkip), countOfRead(0) {
    input1 = &instream;
    input0 = nullptr;
}

template<typename... TupleT>
typename CsvParser<TupleT...>::Iterator CsvParser<TupleT...>::begin() {
    std::string str;

    while (countOfSkip > 0) {
        if (input1 == nullptr) {
            getline(*input0, str);
            ++countOfRead;
        } else {
            getline(*input1, str);
            ++countOfRead;
        }
        countOfSkip--;
    }

    return ++Iterator(&arr, *this, rest);
}

template<typename... TupleT>
typename CsvParser<TupleT...>::Iterator CsvParser<TupleT...>::end() {
    std::tuple<TupleT...> clear;
    std::vector<std::string> clearV;
    return Iterator(&clear, *this, clearV);
}
