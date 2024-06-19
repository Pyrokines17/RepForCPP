#pragma once

#include <iostream>
#include <ostream>
#include <tuple>

template <std::size_t I = 0, typename... TupleT>
typename std::enable_if<(I == sizeof...(TupleT)), void>::type
printElem([[maybe_unused]] const std::tuple<TupleT...>& tt, [[maybe_unused]] std::ostream& os) {
    return;
}

template <std::size_t I = 0, typename... TupleT>
typename std::enable_if<(I < sizeof...(TupleT)), void>::type
printElem(const std::tuple<TupleT...>& tt, std::ostream& os) {
    os << std::get<I>(tt);
    if (I != sizeof...(TupleT) - 1) {
        os << ", ";
    }
    printElem<I + 1>(tt, os);
}

template <std::size_t I = 0, typename... TupleT>
void printElem1(const std::tuple<TupleT...>& tt, std::ostream& os) {
    if constexpr (I == sizeof...(TupleT)) {
        return;
    } else {
        os << std::get<I>(tt);
        if (I != sizeof...(TupleT) - 1) {
            os << ", ";
        }
        printElem1<I + 1>(tt, os);
    }
}

template<typename TupleA, std::size_t... IdT>
void FEFunc(const TupleA& tt, std::ostream& os, std::index_sequence<IdT...>, std::size_t size) {
    ([&os, &tt, &size] {
        os << std::get<IdT>(tt);
        if (IdT != size - 1) {
            os << ", ";
        }
    } (), ...);
}

template <typename TupleA, std::size_t TupleSize = std::tuple_size_v<TupleA>>
void printElem2(const TupleA& tt, std::ostream& os) {
    FEFunc(tt, os, std::make_index_sequence<TupleSize>{}, TupleSize);
}

template<typename... TupleT>
std::ostream& printTuple(std::ostream& os, const std::tuple<TupleT...>& tt) {
    //printElem(tt, os);
    //printElem1(tt, os);
    printElem2(tt, os);
    return os;
}

template<typename... TupleT>
std::ostream& operator << (std::ostream& os, const std::tuple<TupleT...>& tt) {
    return printTuple(os, tt);
}
