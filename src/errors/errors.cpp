#include "errors.hpp"
#include <iostream>
#include <vector>

Error::Error(const std::string type, const std::string details, const size_t error_code, const Position &start, const Position &end) : type(type), details(details), error_code(error_code), start(start), end(end) {}
void Error::print() {
    std::cout << "File '" << this->start.fn << "', at line " << this->start.line << "\nTranspiling process stopped because of error #" << this->error_code << "\n";
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

    size_t end_digits = std::to_string(this->end.line).size();
    if (this->start.line == this->end.line) {
        std::cout << this->start.line << " || " << lines[this->start.line-1] << "\n";
        for (size_t i = 1; i < this->start.column+end_digits+4; i++) {
            std::cout << " ";
        }

        for (size_t i = this->start.column; i < this->end.column; i++) {
            std::cout << "^";
        }

        std::cout << "\n";
    } else {
        for (size_t i = this->start.line-1; i < this->end.line; i++) {
            size_t i_digits = std::to_string(i+1).size();
            for (size_t j = i_digits; j < end_digits; j++) {
                std::cout << 0;
            }
            
            std::cout << i_digits << " || " << lines[i] << "\n";
            for (size_t j = 0; j < end_digits+4; j++) {
                std::cout << " ";
            }

            if (i == this->start.line-1) {
                for (size_t j = 1; j < this->start.column; j++) {
                    std::cout << " ";
                }

                for (size_t j = this->start.column; j <= lines[i].size(); j++) {
                    std::cout << "^";
                }
            } else if (i == this->end.line-1) {
                for (size_t j = 1; j < this->end.column; j++) {
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