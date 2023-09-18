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

    map = new object[other_map.capacity];

    std::copy(other_map.map, other_map.map + other_map.count, map);

    capacity = other_map.capacity;
    count = other_map.count;

    return *this;
};

std::size_t FlatMap::size() const {
    return count;
};

std::string& FlatMap::operator[](const std::string& key) {
    if (count == 0) {
        map[0].key = key;
        count++;

        return map[0].value;
    }

    if (count == capacity) {
        object* altMap = new object[static_cast<std::size_t>(capacity) + newCells];

        std::copy(map, map + count, altMap);

        delete[] map;

        map = altMap; //третий пункт доп.задачи
        capacity += newCells;
    };

    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        id = (id + 1) * -1;

        if (map[id].key != "") {
            shift(map, count, id, "r");
        }

        map[id].key = key;
        count++;
    }

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
        if (id != capacity - 1 && map[id + 1].key != "") {
            shift(map, count, id, "l");
        }

        map[count - 1].key = "";
        map[count - 1].value = "";

        count--;

        return 1;
    }
};

void FlatMap::clear() {
    while (count > 0) { //м.б. сделать удаление-создание мэпы?
        map[count - 1].key = ""; 
        map[count - 1].value = "";

        count--;
    }
};