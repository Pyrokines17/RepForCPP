#include "header.h"

FlatMap::FlatMap() {
    map = new object[5];
    capacity = 5;
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

};

bool FlatMap::contains(const std::string& key) {

};

std::size_t FlatMap::erase(const std::string& key) {

};

void FlatMap::clear() {

};