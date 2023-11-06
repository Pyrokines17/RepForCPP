#pragma once
#include <exception>

class CanNotOpen : public std::exception { //3//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "File not open... ";
    }
};

class CanNotRead : public std::exception { //4//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Read wasn't processed... ";
    }
};

class CanNotWrite : public std::exception { //5//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Write wasn't processed... ";
    }
};

class BadError : public std::exception { //6//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Was sth bad (for example: read not a string) ";
    }
};

class CritError : public std::exception { //7//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Was sth critical (for example: error with disk) ";
    }
};

class RIFFNotFound : public std::exception { //8//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "RIFF wasn't found... ";
    }
};

class ComNotFound : public std::exception { //9//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Some command not allowed... ";
    }
};

class IncorNumOfPar : public std::exception { //10//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Some command have incorrect count of parameters... ";
    }
};

class IncorBordOfPar : public std::exception { //11//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Some command have incorrect time border... ";
    }
};

class ErrorType : public std::exception { //12//
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Some command have incorrect type of parameters... ";
    }
};