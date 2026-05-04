#include "src_helpers.hpp"

std::string generate_indentation(const size_t indentation)  {
    std::string indent = "";
    for (size_t i = 0; i < INDENTATION_SIZE; i++) {
        indent += " ";
    }

    std::string returned = "";
    for (size_t i = 0; i < indentation; i++) {
        returned += indent;
    }

    return returned;
}

std::string generate_position(const Position &position) {
    return std::string("Position(\"") + position.fn + "\", \"\"\"" + position.src + "\"\"\", " + std::to_string(position.column) + ", " + std::to_string(position.line) + ")";
}