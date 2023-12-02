#pragma once
#include <exception>

class CanNotOpen : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "File not open :( ";
    }
};

class CanNotRead : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Read wasn't processed :( ";
    }
};

class CanNotWrite : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Write wasn't processed :( ";
    }
};

class SupOfColor : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Your terminal does not support color :( ";
    }
};