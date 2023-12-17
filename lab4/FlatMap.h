#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

const int newCells = 4;

template <typename KeyT, typename ValueT>
class FlatMap {
    struct object {
        KeyT key;
        ValueT value;
    };

    object* map;
    int capacity;
    int count;

    static int binSearch(object arr[], int low, int high, const KeyT& x);
    static void shift(object* place, int border, int index, const std::string& mode);

    template <typename KeyTS, typename ValueTS>
    friend void swap(FlatMap<KeyTS, ValueTS>& m1, FlatMap<KeyTS, ValueTS>& m2);

public:
    FlatMap();
    FlatMap(const FlatMap& other_map);
    ~FlatMap();

    FlatMap& operator=(const FlatMap& other_map);
    [[nodiscard]] std::size_t size() const;
    ValueT& operator[](const KeyT& key);

    bool contains(const KeyT& key);
    std::size_t erase(const KeyT& key);
    void clear();

    FlatMap(FlatMap&& x) noexcept;
    FlatMap& operator=(FlatMap&& x) noexcept;

    object* begin();
    object* end();
    object* find(const KeyT& x);

};

template <typename KeyT, typename ValueT>
int FlatMap<KeyT, ValueT>::binSearch(object arr[], int low, int high, const KeyT& x) {
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid].key == x) {
            return mid;
        }

        if (arr[mid].key < x) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return (-1 * low - 1);
}

template <typename KeyT, typename ValueT>
void FlatMap<KeyT, ValueT>::shift(object* place, int border, int index, const std::string& mode) {
    if (mode == "r") {
        int end = border;

        while (end != index) {
            place[end] = place[end - 1];
            end--;
        }
    }
    else {
        int begin = index;

        while (begin != border - 1) {
            place[begin] = place[begin + 1];
            begin++;
        }
    }
}

template <typename KeyTS, typename ValueTS>
void swap(FlatMap<KeyTS, ValueTS>& m1, FlatMap<KeyTS, ValueTS>& m2) {
    using std::swap;

    swap(m1.map, m2.map);
    swap(m1.capacity, m2.capacity);
    swap(m1.count, m2.count);
}

template <typename KeyT, typename ValueT>
FlatMap<KeyT, ValueT>::FlatMap() : capacity{ newCells }, count { 0 } {
    map = new object[newCells];
}

template <typename KeyT, typename ValueT>
FlatMap<KeyT, ValueT>::FlatMap(const FlatMap& other_map) : capacity{ other_map.capacity }, count{ other_map.count } {
    map = new object[other_map.capacity];

    std::copy(other_map.map, other_map.map + other_map.count, map);
}

template <typename KeyT, typename ValueT>
FlatMap<KeyT, ValueT>::~FlatMap() {
    delete[] map;
}

template <typename KeyT, typename ValueT>
FlatMap<KeyT, ValueT>& FlatMap<KeyT, ValueT>::operator=(const FlatMap& other_map) {
    if (this == &other_map) {
        return *this;
    }

    FlatMap copy(other_map);
    swap(copy, *this);

    return *this;
}

template <typename KeyT, typename ValueT>
std::size_t FlatMap<KeyT, ValueT>::size() const {
    return count;
}

template <typename KeyT, typename ValueT>
ValueT& FlatMap<KeyT, ValueT>::operator[](const KeyT& key) {
    if (capacity == 0) {
        FlatMap copy(*this);

        copy.map = new object[newCells];
        copy.capacity = newCells;

        swap(copy, *this);
    }

    if (count == 0) {
        map[0].key = key;
        count++;

        return map[0].value;
    }

    if (count == capacity) {
        FlatMap copy(*this);

        auto* altMap = new object[copy.capacity + newCells];
        std::copy(copy.map, copy.map + copy.count, altMap);

        delete[] copy.map;

        copy.map = altMap;
        copy.capacity += newCells;

        swap(copy, *this);
    }

    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        id = (id + 1) * -1;

        shift(map, count, id, "r");

        map[id].key = key;
        count++;
    }

    return map[id].value;
}

template <typename KeyT, typename ValueT>
bool FlatMap<KeyT, ValueT>::contains(const KeyT& key) {
    if (count == 0) {
        return false;
    }

    return binSearch(map, 0, count - 1, key) >= 0;
}

template <typename KeyT, typename ValueT>
std::size_t FlatMap<KeyT, ValueT>::erase(const KeyT& key) {
    if (count == 0) {
        return 0;
    }

    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        return 0;
    }
    else {
        shift(map, count, id, "l");

        map[count - 1].key = KeyT();
        map[count - 1].value = ValueT();
        count--;

        return 1;
    }
}

template <typename KeyT, typename ValueT>
void FlatMap<KeyT, ValueT>::clear() {
    while (count > 0) {
        map[count - 1].key = KeyT();
        map[count - 1].value = ValueT();

        count--;
    }
}

template <typename KeyT, typename ValueT>
FlatMap<KeyT, ValueT>::FlatMap(FlatMap&& x) noexcept : map{ x.map }, capacity{ x.capacity }, count{ x.count } {
    x.map = nullptr;
    x.capacity = 0;
    x.count = 0;
}

template <typename KeyT, typename ValueT>
FlatMap<KeyT, ValueT>& FlatMap<KeyT, ValueT>::operator=(FlatMap&& x) noexcept {
    if (this == &x) {
        return *this;
    }

    FlatMap copy(std::move(x));
    swap(copy, *this);

    return *this;
}

template <typename KeyT, typename ValueT>
typename FlatMap<KeyT, ValueT>::object* FlatMap<KeyT, ValueT>::begin() {
    return map;
}

template <typename KeyT, typename ValueT>
typename FlatMap<KeyT, ValueT>::object* FlatMap<KeyT, ValueT>::end() {
    return map + count;
}

template <typename KeyT, typename ValueT>
typename FlatMap<KeyT, ValueT>::object* FlatMap<KeyT, ValueT>::find(const KeyT& x) {
    if (count == 0) {
        return map;
    }

    int id = binSearch(map, 0, count - 1, x);

    return id < 0 ? map + count : map + id;
}