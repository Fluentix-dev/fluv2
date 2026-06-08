#pragma once
#include "../errors/errors.hpp"
#include "tokens.hpp"
#include <string>
#include <vector>
#include <stack>

struct LexerResult {
    std::vector<Token> tokens;
    Error error;

    LexerResult(const std::vector<Token> tokens, const Error error);
};

struct Lexer {
public:
    std::string src;
    Lexer(const std::string fn, const std::string src);
    LexerResult tokenize();
private:
    Position position;
    size_t index;
    char current_char;
    bool at_line_start;
    std::stack<size_t> indent_stack;
    void advance();
};