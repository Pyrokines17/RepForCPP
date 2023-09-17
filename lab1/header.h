#pragma once

#include <iostream>

class FlatMap {
    struct object {
        std::string key;
        std::string value;
    };

    object* map;

    int capacity;
    int count;

    int binSearch(object arr[], int low, int high, std::string x);
    void shift(object* map, int count, int id);
    void altShift(object* map, int count, int id);

public:
    // стандартный конструктор
    FlatMap();

    // конструктор копирования
    FlatMap(const FlatMap& other_map);

    // деструктор
    ~FlatMap();

    // оператор присваивания
    FlatMap& operator=(const FlatMap& other_map);

    // получить количество элементов в таблице
    std::size_t size() const;

    // доступ / вставка элемента по ключу
    std::string& operator[](const std::string& key);

    // возвращает true, если запись с таким ключом присутствует в таблице
    bool contains(const std::string& key);

    // удаление элемента по ключу, возвращает количество удаленных элементов (0 или 1)
    std::size_t erase(const std::string& key);

    // очистка таблицы, после которой size() возвращает 0, а contains() - false на любой ключ
    void clear();

};