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

    //����������� �����������
    FlatMap(FlatMap&& x) noexcept;

    //�������� �����������
    FlatMap& operator=(FlatMap&& x) noexcept;

    // �������� �������� �� ������ �������
    object* begin();

    // �������� �������� �� �������, ��������� �� ���������
    object* end();

    // �������� �������� �� ������� �� ������� �����, ��� �� end(), ���� ������ ����� ���.
    // � ������� �� operator[] �� ������� ������ ��� ����� �����, ���� � ��� ���
    object* find(const std::string& x);

};