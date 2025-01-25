#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <initializer_list>

const int newCells = 4;

template<typename KeyT, typename ValueT>
struct object {
    KeyT key;
    ValueT value;
};

template <typename KeyT, typename ValueT,
        class Compare = std::less<KeyT>,
        class Allocator = std::allocator<object<KeyT, ValueT>>>
class FlatMap {
    Allocator alloc;
    object<KeyT, ValueT>* map;
    int capacity;
    int count;

    using traits = std::allocator_traits<decltype(alloc)>;

    static int binSearch(object<KeyT, ValueT> arr[], int low, int high, const KeyT& x, Compare comp = Compare());
    static void shift(object<KeyT, ValueT>* place, int border, int index, const std::string& mode);

    template <typename KeyTS, typename ValueTS>
    friend void swap(FlatMap<KeyTS, ValueTS>& m1, FlatMap<KeyTS, ValueTS>& m2);

public:
    FlatMap();
    FlatMap(const FlatMap& other_map);
    ~FlatMap();

    FlatMap& operator = (const FlatMap& other_map);
    [[nodiscard]] std::size_t size() const;
    ValueT& operator [] (const KeyT& key);

    bool contains(const KeyT& key);
    std::size_t erase(const KeyT& key);
    void clear();

    FlatMap(FlatMap&& x) noexcept;
    FlatMap& operator = (FlatMap&& x) noexcept;

    object<KeyT, ValueT>* begin();
    object<KeyT, ValueT>* end();
    object<KeyT, ValueT>* find(const KeyT& x);

};

template <typename KeyT, typename ValueT, class Compare, class Allocator>
int FlatMap<KeyT, ValueT, Compare, Allocator>::binSearch(object<KeyT, ValueT> arr[], int low, int high, const KeyT& x, Compare comp) {
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid].key == x) {
            return mid;
        }

        if (comp(arr[mid].key, x)) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return (-1 * low - 1);
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
void FlatMap<KeyT, ValueT, Compare, Allocator>::shift(object<KeyT, ValueT>* place, int border, int index, const std::string& mode) {
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

template <typename KeyT, typename ValueT, class Compare, class Allocator>
FlatMap<KeyT, ValueT, Compare, Allocator>::FlatMap() : capacity{ newCells }, count { 0 } {
    //map = new object<KeyT, ValueT>[newCells];
    map = traits::allocate(alloc, newCells);
    for (int i = 0; i < newCells; ++i) {
        traits::construct(alloc, map + i, object<KeyT, ValueT>());
    }
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
FlatMap<KeyT, ValueT, Compare, Allocator>::FlatMap(const FlatMap& other_map) : capacity{ other_map.capacity }, count{ other_map.count } {
    //map = new object<KeyT, ValueT>[other_map.capacity];
    map = traits::allocate(alloc, other_map.capacity);
    for (int i = 0; i < other_map.capacity; ++i) {
        traits::construct(alloc, map + i, object<KeyT, ValueT>());
    }

    std::copy(other_map.map, other_map.map + other_map.count, map);
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
FlatMap<KeyT, ValueT, Compare, Allocator>::~FlatMap() {
    //delete[] map;

    for (int i = 0; i < capacity; ++i) {
        traits::destroy(alloc, map + i);
    }

}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
FlatMap<KeyT, ValueT, Compare, Allocator>& FlatMap<KeyT, ValueT, Compare, Allocator>::operator=(const FlatMap& other_map) {
    if (this == &other_map) {
        return *this;
    }

    FlatMap copy(other_map);
    swap(copy, *this);

    return *this;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
std::size_t FlatMap<KeyT, ValueT, Compare, Allocator>::size() const {
    return count;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
ValueT& FlatMap<KeyT, ValueT, Compare, Allocator>::operator [] (const KeyT& key) {
    if (capacity == 0) {
        FlatMap copy(*this);

        //copy.map = new object<KeyT, ValueT>[newCells];
        copy.map = traits::allocate(alloc, newCells);
        for (int i = 0; i < newCells; ++i) {
            traits::construct(alloc, copy.map + i, object<KeyT, ValueT>());
        }

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

        //auto altMap = new object<KeyT, ValueT>[copy.capacity + newCells];
        auto altMap = traits::allocate(alloc, copy.capacity + newCells);
        for (int i = 0; i < copy.capacity + newCells; ++i) {
            traits::construct(alloc, map + i, object<KeyT, ValueT>());
        }

        std::copy(copy.map, copy.map + copy.count, altMap);

        //delete[] copy.map;
        for (int i = 0; i < copy.capacity; ++i) {
            traits::destroy(alloc, copy.map + i);
        }

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

template <typename KeyT, typename ValueT, class Compare, class Allocator>
bool FlatMap<KeyT, ValueT, Compare, Allocator>::contains(const KeyT& key) {
    if (count == 0) {
        return false;
    }

    return binSearch(map, 0, count - 1, key) >= 0;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
std::size_t FlatMap<KeyT, ValueT, Compare, Allocator>::erase(const KeyT& key) {
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

template <typename KeyT, typename ValueT, class Compare, class Allocator>
void FlatMap<KeyT, ValueT, Compare, Allocator>::clear() {
    while (count > 0) {
        map[count - 1].key = KeyT();
        map[count - 1].value = ValueT();

        count--;
    }
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
FlatMap<KeyT, ValueT, Compare, Allocator>::FlatMap(FlatMap&& x) noexcept : map{ x.map }, capacity{ x.capacity }, count{ x.count } {
    x.map = nullptr;
    x.capacity = 0;
    x.count = 0;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
FlatMap<KeyT, ValueT, Compare, Allocator>& FlatMap<KeyT, ValueT, Compare, Allocator>::operator=(FlatMap&& x) noexcept {
    if (this == &x) {
        return *this;
    }

    FlatMap copy(std::move(x));
    swap(copy, *this);

    return *this;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
typename ::object<KeyT, ValueT>* FlatMap<KeyT, ValueT, Compare, Allocator>::begin() {
    return map;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
typename ::object<KeyT, ValueT>* FlatMap<KeyT, ValueT, Compare, Allocator>::end() {
    return map + count;
}

template <typename KeyT, typename ValueT, class Compare, class Allocator>
typename ::object<KeyT, ValueT>* FlatMap<KeyT, ValueT, Compare, Allocator>::find(const KeyT& x) {
    if (count == 0) {
        return map;
    }

    int id = binSearch(map, 0, count - 1, x);

    return id < 0 ? map + count : map + id;
}