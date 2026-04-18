#pragma once

enum struct StatementType {
    Block,
    Expression
};

struct Statement {
    StatementType type;
};

enum struct ExpressionType {
    Binary,
    Unary,
    IntLiteral,
    FloatLiteral
};

struct Expression {
    ExpressionType type;
};