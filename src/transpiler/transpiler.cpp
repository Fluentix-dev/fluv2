#include "transpiler.hpp"
#include "src_helpers/src_helpers.hpp"

Transpiler::Transpiler(const std::string &fn, const std::string &src, const std::shared_ptr<BlockStatement> &ast) : fn(fn), src(src), ast(ast) {}

std::string Transpiler::transpile() {
    std::string main_code = this->block(this->ast, STARTING_INDENTATION);
    return STARTING_SRC + main_code + generate_if_main_program(fn, src);
}

std::string Transpiler::block(const std::shared_ptr<BlockStatement> &block, const size_t indentation) {
    std::string indents = generate_indentation(indentation);
    if (block->body.empty()) {
        return indents + "pass";
    }

    std::string returned = "";
    for (const std::shared_ptr<Statement> &statement : block->body) {
        if (statement->type == StatementType::Block) {
            std::shared_ptr<BlockStatement> block_ = std::static_pointer_cast<BlockStatement>(statement);
            returned += this->block(block_, indentation);
            returned += "\n";
            continue;
        }

        if (statement->type == StatementType::Expression) {
            std::shared_ptr<ExpressionStatement> expression = std::static_pointer_cast<ExpressionStatement>(statement);
            returned += this->expression(expression, indentation);
            returned += "\n";
            continue;
        }

        if (statement->type == StatementType::VariableDeclaration) {
            std::shared_ptr<VariableDeclarationStatement> variable_declaration = std::static_pointer_cast<VariableDeclarationStatement>(statement);
            returned += this->variable_declaration(variable_declaration, indentation);
            returned += "\n";
            continue;
        }

        if (statement->type == StatementType::Assignment) {
            std::shared_ptr<AssignmentStatement> assignment = std::static_pointer_cast<AssignmentStatement>(statement);
            returned += this->assignment(assignment, indentation);
            returned += "\n";
            continue;
        }
    }

    return returned;
}

std::string Transpiler::expression(const std::shared_ptr<ExpressionStatement> &expression, const size_t indentation) {
    std::string indents = generate_indentation(indentation);
    return indents + "print(" + this->expression_(expression->expression) + ".value)";
}

std::string Transpiler::variable_declaration(const std::shared_ptr<VariableDeclarationStatement> &variable_declaration, const size_t indentation) {
    std::string indents = generate_indentation(indentation);
    return indents + "self.scope.declare(" + generate_boolean(variable_declaration->is_constant) + ", " + convert_to_code_string(variable_declaration->variable_name) + ", " + this->expression_(variable_declaration->value) + ", " + generate_position(variable_declaration->start) + ", " + generate_position(variable_declaration->end) + ")";
}

std::string Transpiler::assignment(const std::shared_ptr<AssignmentStatement> &assignment, const size_t indentation) {
    std::string indents = generate_indentation(indentation);
    if (assignment->assigner->type == ExpressionType::Identifier) {
        std::shared_ptr<IdentifierExpression> variable = std::static_pointer_cast<IdentifierExpression>(assignment->assigner);
        return indents + "self.scope.assign(" + convert_to_code_string(variable->identifier) + ", " + this->expression_(assignment->value) + ", " + generate_position(assignment->start) + ", " + generate_position(assignment->end) + ")";
    }

    return "";
}

std::string Transpiler::expression_(const std::shared_ptr<Expression> &expression) {
    if (expression->type == ExpressionType::Binary) {
        std::shared_ptr<BinaryExpression> binary = std::static_pointer_cast<BinaryExpression>(expression);
        return this->binary(binary);
    }

    if (expression->type == ExpressionType::Unary) {
        std::shared_ptr<UnaryExpression> unary = std::static_pointer_cast<UnaryExpression>(expression);
        return this->unary(unary);
    }

    if (expression->type == ExpressionType::IntLiteral) {
        std::shared_ptr<IntExpression> int_ = std::static_pointer_cast<IntExpression>(expression);
        return this->int_(int_);
    }
    
    if (expression->type == ExpressionType::FloatLiteral) {
        std::shared_ptr<FloatExpression> float_ = std::static_pointer_cast<FloatExpression>(expression);
        return this->float_(float_);
    }

    if (expression->type == ExpressionType::Identifier) {
        std::shared_ptr<IdentifierExpression> identifier = std::static_pointer_cast<IdentifierExpression>(expression);
        return this->identifier(identifier);
    }

    if (expression->type == ExpressionType::Group) {
        std::shared_ptr<GroupExpression> group = std::static_pointer_cast<GroupExpression>(expression);
        return this->group(group);
    } 

    return "";
}

std::string Transpiler::binary(const std::shared_ptr<BinaryExpression> &binary) {
    std::string left = this->expression_(binary->left);
    std::string right = this->expression_(binary->right);

    if (binary->op == "+") {
        return left + ".add(" + right + ")";
    }

    if (binary->op == "-") {
        return left + ".sub(" + right + ")";
    }

    if (binary->op == "*") {
        return left + ".mul(" + right + ")";
    }
    
    return left + ".div(" + right + ")";
}

std::string Transpiler::unary(const std::shared_ptr<UnaryExpression> &unary) {
    if (unary->op == "+") {
        return this->expression_(unary->value) + ".positive(" + generate_position(unary->start) + ")";
    }

    return this->expression_(unary->value) + ".negative(" + generate_position(unary->start) + ")";
}

std::string Transpiler::int_(const std::shared_ptr<IntExpression> &int_) {
    return "verifier.verify_int(\"" + int_->literal + "\", " + generate_position(int_->start) + ", " + generate_position(int_->end) + ")";
}

std::string Transpiler::float_(const std::shared_ptr<FloatExpression> &float_) {
    return "verifier.verify_float(\"" + float_->literal + "\", " + generate_position(float_->start) + ", " + generate_position(float_->end) + ")";
}

std::string Transpiler::identifier(const std::shared_ptr<IdentifierExpression> &identifier) {
    return "self.scope.get(" + convert_to_code_string(identifier->identifier) + ", " + generate_position(identifier->start) + ", " + generate_position(identifier->end) + ")";
}

std::string Transpiler::group(const std::shared_ptr<GroupExpression> &group) {
    return this->expression_(group->inside) + ".set_position(" + generate_position(group->start) + ", " + generate_position(group->end) + ")";
}