#include "FlatMap.h"
#include <algorithm>

int FlatMap::binSearch(object arr[], int low, int high, const std::string& x) { 
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
};

void FlatMap::shift(object* place, int border, int index, const std::string& mode) { 
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

void swap(FlatMap& m1, FlatMap& m2) {
    using std::swap;

    swap(m1.map, m2.map);
    swap(m1.capacity, m2.capacity);
    swap(m1.count, m2.count);
}

FlatMap::FlatMap() : capacity{ newCells }, count { 0 } {
    map = new object[newCells];
};

FlatMap::FlatMap(const FlatMap& other_map) : capacity{ other_map.capacity }, count{ other_map.count } {
    map = new object[other_map.capacity];

    std::copy(other_map.map, other_map.map + other_map.count, map);
};

FlatMap::~FlatMap() {
    delete[] map;
};

FlatMap& FlatMap::operator=(const FlatMap& other_map) {
    if (this == &other_map) {
        return *this;
    }

    FlatMap copy(other_map);
    swap(copy, *this); 

    return *this;
};

std::size_t FlatMap::size() const {
    return count;
};

std::string& FlatMap::operator[](const std::string& key) {
    if (capacity == 0) {
        map = new object[newCells];
        capacity = newCells;
    }

    if (count == 0) {
        map[0].key = key;
        count++;

        return map[0].value;
    }

    if (count == capacity) {
        object* altMap = new object[capacity + newCells];

        std::copy(map, map + count, altMap);

        delete[] map;

        map = altMap;
        capacity += newCells;
    };

    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        id = (id + 1) * -1;

        shift(map, count, id, "r");

        map[id].key = key;
        count++;
    }

    return map[id].value;
};

bool FlatMap::contains(const std::string& key) {
    if (count == 0) {
        return false;
    }

    return binSearch(map, 0, count - 1, key) >= 0;
};

std::size_t FlatMap::erase(const std::string& key) { 
    if (count == 0) {
        return 0;
    }
    
    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        return 0;
    }
    else {
        shift(map, count, id, "l");

        map[count - 1].key = ""; 
        map[count - 1].value = "";
        count--;

        return 1;
    }
};

void FlatMap::clear() { 
    while (count > 0) { 
        map[count - 1].key = "";
        map[count - 1].value = "";

        count--;
    }
};

FlatMap::FlatMap(FlatMap&& x) noexcept : map{ x.map }, capacity{ x.capacity }, count{ x.count } {
    x.map = nullptr;
    x.capacity = 0;
    x.count = 0;
};

FlatMap& FlatMap::operator=(FlatMap&& x) noexcept { 
    if (this == &x) {
        return *this;
    }

    FlatMap copy(std::move(x));
    swap(copy, *this);

    return *this;
};

FlatMap::object* FlatMap::begin() {
    return map;
};

FlatMap::object* FlatMap::end() {
    return map + count;
};

FlatMap::object* FlatMap::find(const std::string& x) {
    if (count == 0) {
        return map;
    }

    int id = binSearch(map, 0, count - 1, x);

    if (id < 0) {
        return map + count;
    }
    else {
        return map + id;
    }
};