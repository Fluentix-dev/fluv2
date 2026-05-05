#pragma once
#include "../parser/statements.hpp"

struct Transpiler {
public:
    Transpiler(const std::string &fn, const std::string &src, const std::shared_ptr<BlockStatement> &ast);
    std::string transpile();
private:
    std::shared_ptr<BlockStatement> ast;
    std::string fn;
    std::string src;
    std::string block(const std::shared_ptr<BlockStatement> &block, const size_t indentation);
    std::string expression(const std::shared_ptr<ExpressionStatement> &expression, size_t indentation);
    std::string expression_(const std::shared_ptr<Expression> &expression);
    std::string binary(const std::shared_ptr<BinaryExpression> &binary);
    std::string unary(const std::shared_ptr<UnaryExpression> &unary);
    std::string int_(const std::shared_ptr<IntExpression> &int_);
    std::string float_(const std::shared_ptr<FloatExpression> &float_);
    std::string identifier(const std::shared_ptr<IdentifierExpression> &identifier);
    std::string group(const std::shared_ptr<GroupExpression> &group);
};