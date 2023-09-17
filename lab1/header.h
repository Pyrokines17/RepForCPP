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
    // ����������� �����������
    FlatMap();

    // ����������� �����������
    FlatMap(const FlatMap& other_map);

    // ����������
    ~FlatMap();

    // �������� ������������
    FlatMap& operator=(const FlatMap& other_map);

    // �������� ���������� ��������� � �������
    std::size_t size() const;

    // ������ / ������� �������� �� �����
    std::string& operator[](const std::string& key);

    // ���������� true, ���� ������ � ����� ������ ������������ � �������
    bool contains(const std::string& key);

    // �������� �������� �� �����, ���������� ���������� ��������� ��������� (0 ��� 1)
    std::size_t erase(const std::string& key);

    // ������� �������, ����� ������� size() ���������� 0, � contains() - false �� ����� ����
    void clear();

};