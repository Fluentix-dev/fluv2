#include "statements.hpp"

Statement::Statement(const StatementType type, const Position &start, const Position &end) : type(type), start(start), end(end) {}

BlockStatement::BlockStatement(const std::vector<std::shared_ptr<Statement>> &body, const Position &start, const Position &end) : body(body), Statement(StatementType::Block, start, end) {}
ExpressionStatement::ExpressionStatement(const std::shared_ptr<Expression> &expression, const Position &start, const Position &end) : expression(expression), Statement(StatementType::Expression, start, end) {}

StatementResult::StatementResult(const std::shared_ptr<Statement> &node, const Error error) : node(node), error(error) {}