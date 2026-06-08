#include "parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), index(-1), current_token(tokens[0]) {
    this->advance();
}

void Parser::advance() {
    this->index++;
    this->current_token = this->index < this->tokens.size() ? this->tokens[this->index] : *this->tokens.rbegin();
}

StatementResult Parser::parse() {
    std::vector<std::shared_ptr<Statement>> body = {};
    while (this->current_token.type != TokenType::EndOfFile) {
        while (this->current_token.type == TokenType::Newline) {
            this->advance();
        }

        if (this->current_token.type == TokenType::EndOfFile) {
            break;
        }

        StatementResult statement = this->statement();
        if (statement.error.type != "NULL") {
            return statement;
        }

        body.push_back(statement.node);
        if (this->current_token.type != TokenType::Newline && this->current_token.type != TokenType::EndOfFile) {
            return StatementResult(nullptr, Error("Syntax Error", "expected a newline at the end of statement", 22, this->current_token.start, this->current_token.end));
        }
    }

    return StatementResult(std::make_shared<BlockStatement>(body, this->tokens[0].start, this->current_token.end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
}

StatementResult Parser::inner_block() {
    Position start = this->current_token.start;
    std::vector<std::shared_ptr<Statement>> stmt = {};
    std::vector<Error> errs = {};

    while (this->current_token.type == TokenType::Newline) {
        this->advance();
    }

    this->advance();
    while (this->current_token.type != TokenType::EndOfFile && this->current_token.type != TokenType::Dedent) {
        while (this->current_token.type == TokenType::Newline) {
            this->advance();
        }

        if (this->current_token.type == TokenType::EndOfFile) {
            break;
        }

        if (this->current_token.type == TokenType::Dedent) {
            break;
        }
        
        StatementResult sr = this->statement();
        if (sr.error.type != "NULL") {
            return sr;
        }

        stmt.push_back(sr.node);
    }

    Position end = this->current_token.end;
    this->advance();

    std::shared_ptr<BlockStatement> returned = std::make_shared<BlockStatement>(BlockStatement(stmt, start, end));
    return StatementResult(returned, Error("NULL", "", 0, this->current_token.start, this->current_token.start));
}

StatementResult Parser::statement() {
    if (this->current_token.type == TokenType::Variable) {
        return this->variable_declaration();
    }

    if (this->current_token.type == TokenType::Let) {
        return this->let_declaration();
    }

    if (this->current_token.type == TokenType::Constant) {
        return this->constant_declaration();
    }

    if (this->current_token.type == TokenType::If) {
        return this->if_unless_else();
    }

    ExpressionResult expression = this->expression();
    if (expression.node == nullptr) {
        return StatementResult(nullptr, expression.error);
    }
    
    if (this->current_token.type == TokenType::Is) {
        return this->assignment(expression.node);
    }

    return StatementResult(std::make_shared<ExpressionStatement>(expression.node, expression.node->start, expression.node->end), expression.error);
}

StatementResult Parser::variable_declaration() {
    Position start = this->current_token.start;
    this->advance();

    if (this->current_token.type != TokenType::Identifier) {
        return StatementResult(nullptr, Error("Syntax Error", "expected a variable name in variable declaration", 16, this->current_token.start, this->current_token.end));
    }

    std::string variable_name = this->current_token.value;
    this->advance();

    if (this->current_token.type != TokenType::Is) {
        return StatementResult(nullptr, Error("Syntax Error", "expected 'is' in variable declaration", 17, this->current_token.start, this->current_token.end));
    }

    this->advance();
    ExpressionResult value = this->expression();
    if (value.error.type != "NULL") {
        return StatementResult(nullptr, value.error);
    }

    return StatementResult(std::make_shared<VariableDeclarationStatement>(false, variable_name, value.node, start, this->current_token.end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
}

StatementResult Parser::let_declaration() {
    Position start = this->current_token.start;
    this->advance();

    if (this->current_token.type != TokenType::Identifier) {
        return StatementResult(nullptr, Error("Syntax Error", "expected a variable name in variable declaration", 18, this->current_token.start, this->current_token.end));
    }

    std::string variable_name = this->current_token.value;
    this->advance();

    if (this->current_token.type != TokenType::Be) {
        return StatementResult(nullptr, Error("Syntax Error", "expected 'be' in variable declaration", 19, this->current_token.start, this->current_token.end));
    }

    this->advance();
    ExpressionResult value = this->expression();
    if (value.error.type != "NULL") {
        return StatementResult(nullptr, value.error);
    }

    return StatementResult(std::make_shared<VariableDeclarationStatement>(false, variable_name, value.node, start, this->current_token.end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
}

StatementResult Parser::constant_declaration() {
    Position start = this->current_token.start;
    this->advance();

    if (this->current_token.type != TokenType::Identifier) {
        return StatementResult(nullptr, Error("Syntax Error", "expected a variable name in variable declaration", 20, this->current_token.start, this->current_token.end));
    }

    std::string variable_name = this->current_token.value;
    this->advance();

    if (this->current_token.type != TokenType::Is) {
        return StatementResult(nullptr, Error("Syntax Error", "expected 'is' in variable declaration", 21, this->current_token.start, this->current_token.end));
    }

    this->advance();
    ExpressionResult value = this->expression();
    if (value.error.type != "NULL") {
        return StatementResult(nullptr, value.error);
    }

    return StatementResult(std::make_shared<VariableDeclarationStatement>(true, variable_name, value.node, start, this->current_token.end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
}

StatementResult Parser::assignment(const std::shared_ptr<Expression> &node) {
    if (node->type == ExpressionType::Identifier) {
        this->advance();
        if (this->current_token.type != TokenType::Now) {
            return StatementResult(nullptr, Error("Syntax Error", "expected 'now' in variable assignment", 28, this->current_token.start, this->current_token.end));
        }

        this->advance();
        ExpressionResult value = this->expression();
        if (value.error.type != "NULL") {
            return StatementResult(nullptr, value.error);
        }

        return StatementResult(std::make_shared<AssignmentStatement>(node, value.node, node->start, value.node->end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
    }

    return StatementResult(nullptr, Error("Syntax Error", "invalid syntax!", 27, node->start, node->end));
}

StatementResult Parser::if_unless_else() {
    Position start = this->current_token.start;
    this->advance();

    ExpressionResult condition = this->expression();
    if (condition.error.type != "NULL") {
        return StatementResult(nullptr, condition.error);
    }

    if (this->current_token.type != TokenType::Then) {
        return StatementResult(nullptr, Error("Syntax Error", "expected 'then' in if unless else statement", 28, this->current_token.start, this->current_token.end));
    }

    this->advance();
    StatementResult body = StatementResult(nullptr, Error("NULL", "", 0, this->current_token.start, this->current_token.start));
    if (this->current_token.type == TokenType::Newline) {
        body = this->inner_block();
        if (body.error.type != "NULL") {
            return body;
        }
    } else {
        body = this->statement();
        if (body.error.type != "NULL") {
            return body;
        }

        std::vector<std::shared_ptr<Statement>> block = {body.node};
        body.node = std::make_shared<BlockStatement>(block, body.node->start, body.node->end);
        while (this->current_token.type == TokenType::Newline) {
            this->advance();
        }
    }

    if (this->current_token.type != TokenType::Unless && this->current_token.type != TokenType::Else) {
        return StatementResult(std::make_shared<IfUnlessElseStatement>(condition.node, std::static_pointer_cast<BlockStatement>(body.node), nullptr, start, body.node->end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
    }

    if (this->current_token.type == TokenType::Unless) {
        StatementResult chain = this->if_unless_else();
        if (chain.error.type != "NULL") {
            return chain;
        }

        return StatementResult(std::make_shared<IfUnlessElseStatement>(condition.node, std::static_pointer_cast<BlockStatement>(body.node), std::static_pointer_cast<IfUnlessElseStatement>(chain.node), start, chain.node->end), Error("NULL", "", 0, this->current_token.start, this->current_token.start));
    }

    Position else_start = this->current_token.start;
    this->advance();
    if (this->current_token.type != TokenType::Then) {
        return StatementResult(nullptr, Error("Syntax Error", "expected 'then' in if unless else statement", 30, this->current_token.start, this->current_token.end));
    }

    this->advance();
    StatementResult else_body = StatementResult(nullptr, Error("NULL", "", 0, this->current_token.start, this->current_token.start));
    if (this->current_token.type == TokenType::Newline) {
        else_body = this->inner_block();
        if (else_body.error.type != "NULL") {
            return else_body;
        }
    } else {
        else_body = this->statement();
        if (else_body.error.type != "NULL") {
            return else_body;
        }

        std::vector<std::shared_ptr<Statement>> block = {else_body.node};
        else_body.node = std::make_shared<BlockStatement>(block, else_body.node->start, else_body.node->end);
        while (this->current_token.type == TokenType::Newline) {
            this->advance();
        }
    }

    std::string true_identifier = "true";
    return StatementResult(std::make_shared<IfUnlessElseStatement>(condition.node, std::static_pointer_cast<BlockStatement>(body.node), std::make_shared<IfUnlessElseStatement>(std::make_shared<IdentifierExpression>(true_identifier, this->current_token.start, this->current_token.start), std::static_pointer_cast<BlockStatement>(else_body.node), nullptr, else_start, else_body.node->end), start, else_body.node->end), Error("NULL", "", 0, this->current_token.start, this->current_token.end));
}

ExpressionResult Parser::expression() {
    return this->additive();
}

ExpressionResult Parser::additive() {
    ExpressionResult left = this->multiplicative();
    if (left.error.type != "NULL") {
        return left;
    }

    while (this->current_token.type == TokenType::Plus || this->current_token.type == TokenType::Minus) {
        std::string op = this->current_token.value;
        this->advance();
        
        ExpressionResult right = this->multiplicative();
        if (right.error.type != "NULL") {
            return right;
        }

        left.node = std::make_shared<BinaryExpression>(left.node, op, right.node, left.node->start, right.node->end);
    }

    return left;
}

ExpressionResult Parser::multiplicative() {
    ExpressionResult left = this->unary();
    if (left.error.type != "NULL") {
        return left;
    }

    while (this->current_token.type == TokenType::Multiply || this->current_token.type == TokenType::Divide) {
        std::string op = this->current_token.value;
        this->advance();
        
        ExpressionResult right = this->unary();
        if (right.error.type != "NULL") {
            return right;
        }

        left.node = std::make_shared<BinaryExpression>(left.node, op, right.node, left.node->start, right.node->end);
    }

    return left;
}

ExpressionResult Parser::unary() {
    if (this->current_token.type == TokenType::Plus || this->current_token.type == TokenType::Minus) {
        std::string op = this->current_token.value;
        Position start = this->current_token.start;
        this->advance();

        ExpressionResult value = this->unary();
        if (value.error.type != "NULL") {
            return value;
        }

        return ExpressionResult(std::make_shared<UnaryExpression>(op, value.node, start, value.node->end), value.error);
    }

    return this->primary();
}

ExpressionResult Parser::primary() {
    if (this->current_token.type == TokenType::Int) {
        std::string literal = this->current_token.value;
        Position start = this->current_token.start;
        Position end = this->current_token.end;
        this->advance();

        return ExpressionResult(std::make_shared<IntExpression>(literal, start, end), Error("NULL", "", 0, start, end));
    }

    if (this->current_token.type == TokenType::Float) {
        std::string literal = this->current_token.value;
        Position start = this->current_token.start;
        Position end = this->current_token.end;
        this->advance();

        return ExpressionResult(std::make_shared<FloatExpression>(literal, start, end), Error("NULL", "", 0, start, end));
    }

    if (this->current_token.type == TokenType::Identifier) {
        std::string identifier = this->current_token.value;
        Position start = this->current_token.start;
        Position end = this->current_token.end;
        this->advance();

        return ExpressionResult(std::make_shared<IdentifierExpression>(identifier, start, end), Error("NULL", "", 0, start, end));
    }

    if (this->current_token.type == TokenType::LParen) {
        Position start = this->current_token.start;
        this->advance();

        ExpressionResult inside = this->expression();
        if (inside.error.type != "NULL") {
            return inside;
        }

        if (this->current_token.type != TokenType::RParen) {
            return ExpressionResult(nullptr, Error("Syntax Error", "expected ')' to match with '('", 4, this->current_token.start, this->current_token.end));
        }

        Position end = this->current_token.end;
        this->advance();
        return ExpressionResult(std::make_shared<GroupExpression>(inside.node, start, end), inside.error);
    }

    return ExpressionResult(nullptr, Error("Syntax Error", "invalid syntax!", 3, this->current_token.start, this->current_token.end));
}