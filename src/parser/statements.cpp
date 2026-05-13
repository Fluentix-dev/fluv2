#include "statements.hpp"

Statement::Statement(const StatementType type, const Position &start, const Position &end) : type(type), start(start), end(end) {}

BlockStatement::BlockStatement(const std::vector<std::shared_ptr<Statement>> &body, const Position &start, const Position &end) : body(body), Statement(StatementType::Block, start, end) {}
ExpressionStatement::ExpressionStatement(const std::shared_ptr<Expression> &expression, const Position &start, const Position &end) : expression(expression), Statement(StatementType::Expression, start, end) {}
VariableDeclarationStatement::VariableDeclarationStatement(const bool is_constant, const std::string &variable_name, const std::shared_ptr<Expression> &value, const Position &start, const Position &end) : is_constant(is_constant), variable_name(variable_name), value(value), Statement(StatementType::VariableDeclaration, start, end) {}
AssignmentStatement::AssignmentStatement(const std::shared_ptr<Expression> &assigner, const std::shared_ptr<Expression> &value, const Position &start, const Position &end) : assigner(assigner), value(value), Statement(StatementType::Assignment, start, end) {}

StatementResult::StatementResult(const std::shared_ptr<Statement> &node, const Error error) : node(node), error(error) {}