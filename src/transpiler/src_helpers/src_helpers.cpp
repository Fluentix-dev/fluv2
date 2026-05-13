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
    return std::string("Position(self.fn, self.src, ") + std::to_string(position.column) + ", " + std::to_string(position.line) + ")";
}

std::string convert_to_code_string(const std::string string) {
    std::string returned = "\"";
    for (const char &c : string) {
        if (c == '\n') {
            returned += "\\n";
        } else if (c == '\t') {
            returned += "\\t";
        } else if (c == '"') {
            returned += "\\\"";
        } else if (c == '\\') {
            returned += "\\\\";
        } else {
            returned += c;
        }
    }

    return returned + "\"";
}

std::string generate_if_main_program(const std::string &fn, const std::string &src) {
    std::string new_fn = convert_to_code_string(fn);
    std::string new_src = convert_to_code_string(src);

    return R"(
if __name__ == "__main__":
    program = Program()" + new_fn + ", " + new_src + R"()
    program.main())";
}

std::string generate_boolean(const bool value) {
    return (value ? "True" : "False");
}