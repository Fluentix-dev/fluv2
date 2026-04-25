#pragma once
#include "../errors/errors.hpp"
#include <memory>
#include <string>

enum struct ExpressionType {
    Binary,
    Unary,
    IntLiteral,
    FloatLiteral,
    Identifier,
    Group
};

struct Expression {
    ExpressionType type;
    Position start;
    Position end;

    Expression(const ExpressionType type, const Position &start, const Position &end);
};

struct BinaryExpression : public Expression {
    std::shared_ptr<Expression> left;
    std::string op;
    std::shared_ptr<Expression> right;

    BinaryExpression(const std::shared_ptr<Expression> &left, const std::string op, const std::shared_ptr<Expression> &right, const Position &start, const Position &end);
};

struct UnaryExpression : public Expression {
    std::string op;
    std::shared_ptr<Expression> value;

    UnaryExpression(const std::string op, const std::shared_ptr<Expression> &value, const Position &start, const Position &end);
};

struct IntExpression : public Expression {
    std::string literal;
    IntExpression(const std::string &literal, const Position &start, const Position &end);
};

struct FloatExpression : public Expression {
    std::string literal;
    FloatExpression(const std::string &literal, const Position &start, const Position &end);
};

struct IdentifierExpression : public Expression {
    std::string identifier;
    IdentifierExpression(const std::string &identifier, const Position &start, const Position &end);
};

struct ExpressionResult {
    std::shared_ptr<Expression> node;
    Error error;

    ExpressionResult(const std::shared_ptr<Expression> &node, Error error);
};

struct GroupExpression : public Expression {
    std::shared_ptr<Expression> inside;
    GroupExpression(const std::shared_ptr<Expression> &inside, const Position &start, const Position &end);
};