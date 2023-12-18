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
    os << std::get<I>(tt) << " ";
    printElem<I + 1>(tt, os);
}

template<typename... TupleT>
std::ostream& printTuple(std::ostream& os, const std::tuple<TupleT...>& tt) {
    printElem(tt, os);
    return os;
}

template<typename... TupleT>
std::ostream& operator <<(std::ostream& os, const std::tuple<TupleT...>& tt) {
    return printTuple(os, tt);
}