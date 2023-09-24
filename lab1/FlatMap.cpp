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
    if (mode == "r") { //м.б. улучшение сдвига...
        int end = border;

        while (end != index) {
            place[end] = place[end - 1];
            end--;
        }
    }
    else {
        int begin = index;

        while (begin != border - 1) { 
            map[begin] = map[begin + 1];
            begin++;
        }
    }
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

    delete[] map;
    map = nullptr;

    FlatMap copy(other_map);
    std::swap(copy, *this);

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

        std::swap(copy, *this);

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

    std::swap(copy, *this);

    return map[id].value;
};

bool FlatMap::contains(const std::string& key) {
    return binSearch(map, 0, count - 1, key) < 0 ? false : true;
};

std::size_t FlatMap::erase(const std::string& key) { //найти причину прикола
    FlatMap copy(*this);

    int id = binSearch(copy.map, 0, copy.count - 1, key);

    if (id < 0) {
        std::swap(copy, *this); //нужен ли здесь своп...

        return 0;
    }
    else {
        if (id != copy.capacity - 1 && copy.map[id + 1].key != "") {
            shift(copy.map, copy.count, id, "l");
        }

        copy.map[copy.count - 1].key = "";
        copy.map[copy.count - 1].value = "";
        copy.count--;

        std::swap(copy, *this);

        return 1;
    }
};

void FlatMap::clear() {
    FlatMap copy(*this);

    while (copy.count > 0) { //м.б. сделать удаление-создание мэпы?
        copy.map[copy.count - 1].key = ""; 
        copy.map[copy.count - 1].value = "";
        copy.count--;
    }

    std::swap(copy, *this);
};

FlatMap::FlatMap(FlatMap&& x) noexcept {
    map = x.map;
    capacity = x.capacity;
    count = x.count;

    x.map = nullptr;
};

FlatMap& FlatMap::operator=(FlatMap&& x) noexcept {
    if (this == &x) {
        return *this;
    }

    delete[] map;

    map = x.map;
    capacity = x.capacity;
    count = x.count;

    x.map = nullptr;

    return *this;
};