#pragma once

#include <iostream>

#define newCells 4

class FlatMap {
    struct object {
        std::string key;
        std::string value;
    };

    object* map;

    int capacity;
    int count;

    int binSearch(object arr[], int low, int high, const std::string& x);
    void shift(object* place, int border, int index, const std::string& mode);

    friend void swap(FlatMap& m1, FlatMap& m2);

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

    //конструктор перемещения
    FlatMap(FlatMap&& x) noexcept;

    //оператор перемещения
    FlatMap& operator=(FlatMap&& x) noexcept;

    // Получить итератор на первый элемент
    object* begin();

    // Получить итератор на элемент, следующий за последним
    object* end();

    // Получить итератор на элемент по данному ключу, или на end(), если такого ключа нет.
    // В отличие от operator[] не создает записи для этого ключа, если её ещё нет
    object* find(const std::string& x);

};