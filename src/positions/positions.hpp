#pragma once
#include <string>

struct Position {
    std::string fn;
    size_t column;
    size_t line;

    Position(const std::string &fn, const size_t &column, const size_t &line) {
        this->fn = fn;
        this->column = column;
        this->line = line;
    }
};