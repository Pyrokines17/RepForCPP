#pragma once

#include <string>

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