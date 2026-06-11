#pragma once
#include "statements.hpp"
#include "../lexer/tokens.hpp"

bool statement_requires_newline(const std::shared_ptr<Statement> &stmt);

struct Parser {
public:
    Parser(const std::vector<Token> &tokens);
    StatementResult parse();
private:
    std::vector<Token> tokens;
    Token current_token;
    size_t index;

    void advance();

    StatementResult inner_block();
    StatementResult statement();
    StatementResult variable_declaration();
    StatementResult let_declaration();
    StatementResult constant_declaration();
    StatementResult assignment(const std::shared_ptr<Expression> &node);
    StatementResult if_unless_else();
    ExpressionResult expression();
    ExpressionResult additive();
    ExpressionResult multiplicative();
    ExpressionResult unary();
    ExpressionResult primary();
};