#include "header.h"

class FlatMap {
    struct object {
        std::string key;

    private:
        std::string value;

    };

    object* map;

    int capacity;
    int count;

public:

    // стандартный конструктор
    FlatMap() {
        map = new object[5];
        capacity = 5;
        count = 0;
    };

    // конструктор копирования
    FlatMap(const FlatMap& other_map) {
        map = new object[other_map.capacity];

        for (int i = 0; i < other_map.capacity; i++) {
            map[i] = other_map.map[i];
        }

        capacity = other_map.capacity;
        count = other_map.count;
    };

    // деструктор
    ~FlatMap() {
        delete[] map;
    };

    // оператор присваивания
    FlatMap& operator=(const FlatMap& other_map) {
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

    // получить количество элементов в таблице
    std::size_t size() const {
        return count;
    };

    // доступ / вставка элемента по ключу
    std::string& operator[](const std::string& key);

    // возвращает true, если запись с таким ключом присутствует в таблице
    bool contains(const std::string& key);

    // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
    std::size_t erase(const std::string& key);

    // очистка таблицы, после которой size() возвращает 0, а contains() - false на любой ключ
    void clear();

};