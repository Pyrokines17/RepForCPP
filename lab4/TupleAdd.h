#pragma once

#include <iostream>
#include <ostream>

template<typename... Args>
struct MyTuple;

template<typename Head, typename... Tail>
struct MyTuple<Head, Tail...> : MyTuple<Tail...> {
    using valueType = Head;

    MyTuple<Tail...> base = static_cast<MyTuple<Tail...>&>(*this);
    Head head;

    explicit MyTuple(Head h, Tail... t) : MyTuple<Tail...>(t...), head(h) {};
};

template<>
struct MyTuple<> {
};

template<int I, typename Head, typename... Args>
struct Getter {
    typedef typename Getter<I-1, Args...>::returnType returnType;
    static returnType get(const MyTuple<Head, Args...>& t) {
        return Getter<I-1, Args...>::get(t);
    }
};

template<typename Head, typename... Args>
struct Getter<0, Head, Args...> {
    typedef typename MyTuple<Head, Args...>::valueType returnType;
    static returnType get(const MyTuple<Head, Args...>& t) {
        return t.head;
    }
};

template<int I, typename Head, typename... Args>
typename Getter<I, Head, Args...>::returnType
get(const MyTuple<Head, Args...>& t) {
    return Getter<I, Head, Args...>::get(t);
}

template <std::size_t I = 0, typename Head, typename... Tail>
void printElem(const MyTuple<Head, Tail...>& tt, std::ostream& os) {
    if constexpr (I == sizeof...(Tail) + 1) {
        return;
    } else {
        os << get<I>(tt);
        if (I != sizeof...(Tail)) {
            os << ", ";
        }
        printElem<I + 1>(tt, os);
    }
}

template<typename Head, typename... Tail>
std::ostream& printMyTuple(std::ostream& os, const MyTuple<Head, Tail...>& tt) {
    printElem(tt, os);
    return os;
}

template<typename Head, typename... Tail>
std::ostream& operator << (std::ostream& os, const MyTuple<Head, Tail...>& tt) {
    return printMyTuple(os, tt);
}