#include "errors.hpp"
#include <iostream>
#include <vector>

Error::Error(const std::string type, const std::string details, const size_t error_code, const Position &start, const Position &end) : type(type), details(details), error_code(error_code), start(start), end(end) {}
void Error::print() {
    std::cout << "File '" << start.fn << "', at line " << start.line << "\nTranspiling process stopped because of error #" << this->error_code << "\n";
    std::vector<std::string> lines = {};
    std::string current_line = "";
    for (const char &c : this->start.src + "\n") {
        if (c == '\n') {
            lines.push_back(current_line);
            current_line = "";
        } else {
            current_line += c;
        }
    }

    size_t end_digits = std::to_string(end.line).size();
    if (start.line == end.line) {
        std::cout << start.line << " || " << lines[start.line-1] << "\n";
        for (size_t i = 1; i < start.column+end_digits+4; i++) {
            std::cout << " ";
        }

        for (size_t i = start.column; i < end.column; i++) {
            std::cout << "^";
        }

        std::cout << "\n";
    } else {
        for (size_t i = start.line-1; i < end.line; i++) {
            size_t i_digits = std::to_string(i+1).size();
            for (size_t j = i_digits; j < end_digits; j++) {
                std::cout << 0;
            }
            
            std::cout << i_digits << " || " << lines[i] << "\n";
            for (size_t j = 0; j < end_digits+4; j++) {
                std::cout << " ";
            }

            if (i == start.line-1) {
                for (size_t j = 1; j < start.column; j++) {
                    std::cout << " ";
                }

                for (size_t j = start.column; j <= lines[i].size(); j++) {
                    std::cout << "^";
                }
            } else if (i == end.line-1) {
                for (size_t j = 1; j < end.column; j++) {
                    std::cout << "^";
                }
            } else {
                for (const char &c : lines[i]) {
                    std::cout << "^";
                }
            }

            std::cout << "\n";
        }
    }

    std::cout << this->type << ": " << this->details << "\n";
}