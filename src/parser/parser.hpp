#pragma once
#include "statements.hpp"
#include "../lexer/tokens.hpp"

struct Parser {
public:
    Parser(const std::vector<Token> &tokens);
    StatementResult parse();
private:
    std::vector<Token> tokens;
    Token current_token;
    size_t index;

    void advance();

    StatementResult statement();
    ExpressionResult expression();
    ExpressionResult additive();
    ExpressionResult multiplicative();
    ExpressionResult unary();
    ExpressionResult primary();
};