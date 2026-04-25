#pragma once
#include "expressions.hpp"
#include <vector>

enum struct StatementType {
    Block,
    Expression
};

struct Statement {
    StatementType type;
    Position start;
    Position end;

    Statement(const StatementType type, const Position &start, const Position &end);
};

struct BlockStatement : public Statement {
    std::vector<std::shared_ptr<Statement>> body;
    BlockStatement(const std::vector<std::shared_ptr<Statement>> &body, const Position &start, const Position &end);
};

struct ExpressionStatement : public Statement {
    std::shared_ptr<Expression> expression;
    ExpressionStatement(const std::shared_ptr<Expression> &expression, const Position &start, const Position &end);
};

struct StatementResult {
    std::shared_ptr<Statement> node;
    Error error;

    StatementResult(const std::shared_ptr<Statement> &node, const Error error);
};