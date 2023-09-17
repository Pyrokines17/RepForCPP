#include "Header.h"

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

void FlatMap::shift(object* map, int count, int id) {
    int end = count;

    while (end != id) {
        map[end].key = map[end - 1].key;
        map[end].value = map[end - 1].value;

        end--;
    }
}

void FlatMap::altShift(object* map, int count, int id) {
    int begin = id;

    while (begin != count - 1) {
        map[begin].key = map[begin + 1].key;
        map[begin].value = map[begin + 1].value;

        begin++;
    }
}

FlatMap::FlatMap() {
    map = new object[4];
    capacity = 4;
    count = 0;
};

FlatMap::FlatMap(const FlatMap& other_map) {
    map = new object[other_map.capacity];

    for (int i = 0; i < other_map.capacity; i++) {
        map[i] = other_map.map[i];
    }

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

    for (int i = 0; i < other_map.capacity; i++) {
        map[i] = other_map.map[i];
    }

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
        object* altMap = new object[capacity + 4];
        memcpy(altMap, map, count * sizeof(object));
        delete[] map;

        map = altMap;
        capacity += 4;
    };

    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        id = (id + 1) * -1;

        if (map[id].key != "") {
            shift(map, count, id);
        }

        map[id].key = key;
        count++;
    }

    return map[id].value;
};

bool FlatMap::contains(const std::string& key) {
    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        return false;
    }
    else {
        return true;
    }
};

std::size_t FlatMap::erase(const std::string& key) {
    int id = binSearch(map, 0, count - 1, key);

    if (id < 0) {
        return 0;
    }
    else {
        if (id != capacity - 1 && map[id + 1].key != "") {
            altShift(map, count, id);
        }

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