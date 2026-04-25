#pragma once
#include "../positions/positions.hpp"

struct Error {
    std::string type;
    std::string details;
    size_t error_code;
    Position start;
    Position end;

    Error(const std::string type, const std::string details, const size_t error_code, const Position &start, const Position &end);
    void print();
};