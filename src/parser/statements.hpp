#pragma once
#include "expressions.hpp"
#include <vector>

enum struct StatementType {
    Block,
    Expression,
    VariableDeclaration,
    Assignment,
    IfUnlessElse,
    Repeat
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

struct VariableDeclarationStatement : public Statement {
    bool is_constant;
    std::string variable_name;
    std::shared_ptr<Expression> value;

    VariableDeclarationStatement(const bool is_constant, const std::string &variable_name, const std::shared_ptr<Expression> &value, const Position &start, const Position &end);
};

struct AssignmentStatement : public Statement {
    std::shared_ptr<Expression> assigner;
    std::shared_ptr<Expression> value;

    AssignmentStatement(const std::shared_ptr<Expression> &assigner, const std::shared_ptr<Expression> &value, const Position &start, const Position &end);
};

struct IfUnlessElseStatement : public Statement {
    std::shared_ptr<Expression> condition;
    std::shared_ptr<BlockStatement> body;
    std::shared_ptr<IfUnlessElseStatement> next;

    IfUnlessElseStatement(const std::shared_ptr<Expression> &condition, const std::shared_ptr<BlockStatement> &body, const std::shared_ptr<IfUnlessElseStatement> next, const Position &start, const Position &end);
};

struct RepeatStatement : public Statement {
    std::shared_ptr<Expression> condition;
    std::shared_ptr<BlockStatement> body;

    RepeatStatement(const std::shared_ptr<Expression> &condition, const std::shared_ptr<BlockStatement> &body, const Position &start, const Position &end);
};

struct StatementResult {
    std::shared_ptr<Statement> node;
    Error error;

    StatementResult(const std::shared_ptr<Statement> &node, const Error error);
};