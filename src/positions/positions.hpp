#pragma once
#include <string>

struct Position {
    std::string fn;
    std::string src;
    size_t column;
    size_t line;

    Position(const std::string &fn, const std::string &src, const size_t column, const size_t line);
};