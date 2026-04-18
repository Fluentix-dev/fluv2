#include "positions.hpp"

Position::Position(const std::string &fn, const std::string &src, const size_t column, const size_t line) : fn(fn), src(src), column(column), line(line) {}