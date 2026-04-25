#include "expressions.hpp"

Expression::Expression(const ExpressionType type, const Position &start, const Position &end) : type(type), start(start), end(end) {}

BinaryExpression::BinaryExpression(const std::shared_ptr<Expression> &left, const std::string op, const std::shared_ptr<Expression> &right, const Position &start, const Position &end) : left(left), op(op), right(right), Expression(ExpressionType::Binary, start, end) {}
UnaryExpression::UnaryExpression(const std::string op, const std::shared_ptr<Expression> &value, const Position &start, const Position &end) : op(op), value(value), Expression(ExpressionType::Unary, start, end) {}
IntExpression::IntExpression(const std::string &literal, const Position &start, const Position &end) : literal(literal), Expression(ExpressionType::IntLiteral, start, end) {}
FloatExpression::FloatExpression(const std::string &literal, const Position &start, const Position &end) : literal(literal), Expression(ExpressionType::FloatLiteral, start, end) {}
IdentifierExpression::IdentifierExpression(const std::string &identifier, const Position &start, const Position &end) : identifier(identifier), Expression(ExpressionType::Identifier, start, end) {}
GroupExpression::GroupExpression(const std::shared_ptr<Expression> &inside, const Position &start, const Position &end) : inside(inside), Expression(ExpressionType::Group, start, end) {}

ExpressionResult::ExpressionResult(const std::shared_ptr<Expression> &node, const Error error) : node(node), error(error) {}