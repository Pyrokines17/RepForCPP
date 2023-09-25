#include "FlatMap.h"
#include <algorithm>

int FlatMap::binSearch(object arr[], int low, int high, std::string x) { 
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

    return (-1 * (low > high ? low : high) - 1);
};

void FlatMap::shift(object* place, int border, int index, std::string mode) { 
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

FlatMap::FlatMap() {
    map = new object[newCells];
    capacity = newCells;
    count = 0;
};

FlatMap::FlatMap(const FlatMap& other_map) {
    map = new object[other_map.capacity];

    std::copy(other_map.map, other_map.map + other_map.count, map);

    capacity = other_map.capacity;
    count = other_map.count;
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
    FlatMap copy(*this);

    if (copy.count == 0) {
        copy.map[0].key = key;
        copy.count++;

        swap(copy, *this);

        return map[0].value;
    }

    if (copy.count == copy.capacity) {
        object* altMap = new object[static_cast<std::size_t>(copy.capacity) + newCells];
        std::copy(copy.map, copy.map + copy.count, altMap);

        delete[] copy.map;

        copy.map = altMap;
        copy.capacity += newCells;
    };

    int id = binSearch(copy.map, 0, copy.count - 1, key);

    if (id < 0) {
        id = (id + 1) * -1;

        if (copy.map[id].key != "") {
            shift(copy.map, copy.count, id, "r");
        }

        copy.map[id].key = key;
        copy.count++;
    }

    swap(copy, *this);

    return map[id].value;
};

bool FlatMap::contains(const std::string& key) {
    return binSearch(map, 0, count - 1, key) < 0 ? false : true;
};

std::size_t FlatMap::erase(const std::string& key) { 
    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        return 0;
    }
    else {
        FlatMap copy(*this);

        if (id != copy.capacity - 1 && copy.map[id + 1].key != "") {
            shift(copy.map, copy.count, id, "l");
        }

        copy.map[copy.count - 1].key = ""; 
        copy.map[copy.count - 1].value = "";
        copy.count--;

        swap(copy, *this);

        return 1;
    }
};

void FlatMap::clear() { 
    FlatMap copy(*this);

    delete[] copy.map;
    copy.map = new object[copy.capacity];
    copy.count = 0;

    swap(copy, *this);
};

FlatMap::FlatMap(FlatMap&& x) noexcept {
    map = x.map;
    capacity = x.capacity;
    count = x.count;

    x.map = nullptr;
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