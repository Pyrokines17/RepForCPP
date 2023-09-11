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

    // ����������� �����������
    FlatMap() {
        map = new object[5];
        capacity = 5;
        count = 0;
    };

    // ����������� �����������
    FlatMap(const FlatMap& other_map) {
        map = new object[other_map.capacity];

        for (int i = 0; i < other_map.capacity; i++) {
            map[i] = other_map.map[i];
        }

        capacity = other_map.capacity;
        count = other_map.count;
    };

    // ����������
    ~FlatMap() {
        delete[] map;
    };

    // �������� ������������
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

    // �������� ���������� ��������� � �������
    std::size_t size() const {
        return count;
    };

    // ������ / ������� �������� �� �����
    std::string& operator[](const std::string& key);

    // ���������� true, ���� ������ � ����� ������ ������������ � �������
    bool contains(const std::string& key);

    // �������� �������� �� �����, ���������� ���������� ��������� ��������� (0 ��� 1)
    std::size_t erase(const std::string& key);

    // ������� �������, ����� ������� size() ���������� 0, � contains() - false �� ����� ����
    void clear();

};