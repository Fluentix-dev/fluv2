#include "lexer.hpp"
#include <unordered_map>

LexerResult::LexerResult(const std::vector<Token> tokens, const Error error) : tokens(tokens), error(error) {}
Lexer::Lexer(const std::string fn, const std::string src) : src(src), position(Position(fn, src, 0, 0)), index(-1), current_char('\n'), at_line_start(false) {
    this->advance();
}

void Lexer::advance() {
    if (this->current_char == '\n') {
        this->position.line++;
        this->position.column = 0;
    }

    if (++this->index < this->src.size()) {
        this->current_char = this->src[this->index];
    } else {
        this->current_char = '\0';
    }

    this->position.column++;
}

LexerResult Lexer::tokenize() {
    while (!this->indent_stack.empty()) {
        this->indent_stack.pop();
    }

    this->indent_stack.push(0);
    std::unordered_map<std::string, TokenType> keywords = {
        {"variable", TokenType::Variable},
        {"let", TokenType::Let},
        {"constant", TokenType::Constant},
        {"is", TokenType::Is},
        {"be", TokenType::Be},
        {"now", TokenType::Now},
        {"null", TokenType::Null},
        {"get", TokenType::Get},
        {"module", TokenType::Module},
        {"if", TokenType::If},
        {"unless", TokenType::Unless},
        {"elif", TokenType::Elif},
        {"else", TokenType::Else},
        {"define", TokenType::Define},
        {"with", TokenType::With},
        {"return", TokenType::Return},
        {"repeat", TokenType::Repeat},
        {"until", TokenType::Until},
        {"stop", TokenType::Stop},
        {"include", TokenType::Include},
        {"to", TokenType::To},
        {"exclude", TokenType::Exclude},
        {"from", TokenType::From},
        {"element", TokenType::Element},
        {"at", TokenType::At},
        {"then", TokenType::Then}
    };

    std::vector<Token> tokens = {};
    while (this->current_char != '\0') {
        if (this->at_line_start) {
            Position start = this->position;
            size_t indent_level = 0;
            while (this->current_char == ' ' || this->current_char == '\t') {
                if (this->current_char == ' ') {
                    indent_level++;
                } else if (this->current_char == '\t') {
                    indent_level += 4;
                }

                this->advance();
            }
            
            if (this->current_char == '\n' || this->current_char == '\0') {
                if (this->current_char == '\n') {
                    this->advance();
                }

                this->at_line_start = true;
                continue;
            }
            
            if (indent_level > this->indent_stack.top()) {
                this->indent_stack.push(indent_level);
                tokens.push_back(Token(TokenType::Indent, "INDENT", start, this->position));
            } else if (indent_level < this->indent_stack.top()) {
                while (!this->indent_stack.empty() && this->indent_stack.top() > indent_level) {
                    this->indent_stack.pop();
                    tokens.push_back(Token(TokenType::Dedent, "DEDENT", start, this->position));
                }

                if (this->indent_stack.empty() || this->indent_stack.top() != indent_level) {
                    return LexerResult({}, Error("Syntax Error", "unindent does not match any outer indentation level", 29, start, this->position));
                }
            }
            
            this->at_line_start = false;
            continue;
        }

        if (this->current_char == '\n') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::Newline, "\\n", start, this->position));
            this->at_line_start = true;
            continue;
        }

        if (this->current_char == ' ' || this->current_char == '\t') {
            this->advance();
            continue;
        }

        if (this->current_char == '+') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::Plus, "+", start, this->position));
            continue;
        }

        if (this->current_char == '-') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::Minus, "-", start, this->position));
            continue;
        }

        if (this->current_char == '*') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::Multiply, "*", start, this->position));
            continue;
        }

        if (this->current_char == '/') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::Divide, "/", start, this->position));
            continue;
        }

        if (this->current_char == '(') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::LParen, "(", start, this->position));
            continue;
        }

        if (this->current_char == ')') {
            Position start = this->position;
            this->advance();
            tokens.push_back(Token(TokenType::RParen, ")", start, this->position));
            continue;
        }

        if (this->current_char == '.' || ('0' <= this->current_char && this->current_char <= '9')) {
            std::string number = "";
            Position start = this->position;
            bool dot = this->current_char == '.';
            bool dot2 = false;
            while (this->current_char == '.' || ('0' <= this->current_char && this->current_char <= '9')) {
                if (this->current_char == '.') {
                    if (dot) {
                        dot2 = true;
                    }

                    dot = true;
                }

                number += this->current_char;
                this->advance();
            }

            if (dot2) {
                return LexerResult({}, Error("Syntax Error", "we don't accept version numbers without quotes like that...", 1, start, this->position));
            }

            if (dot) {
                tokens.push_back(Token(TokenType::Float, number, start, this->position));
            } else {
                tokens.push_back(Token(TokenType::Int, number, start, this->position));
            }

            continue;
        }

        if (this->current_char == '_' || ('A' <= this->current_char && this->current_char <= 'Z') || ('a' <= this->current_char && this->current_char <= 'z')) {
            std::string identifier = "";
            Position start = this->position;
            while (this->current_char == '_' || ('A' <= this->current_char && this->current_char <= 'Z') || ('a' <= this->current_char && this->current_char <= 'z') || ('0' <= this->current_char && this->current_char <= '9')) {
                identifier += this->current_char;
                this->advance();
            }

            if (keywords.count(identifier) > 0) {
                tokens.push_back(Token(keywords[identifier], identifier, start, this->position));
            } else {
                tokens.push_back(Token(TokenType::Identifier, identifier, start, this->position));
            }

            continue;
        }

        char character = this->current_char;
        Position start = this->position;
        this->advance();
        return LexerResult({}, Error("Syntax Error", std::string("unexpected character: '") + character + "'", 2, start, this->position));
    }

    Position start = this->position;
    this->advance();

    while (this->indent_stack.top() > 0) {
        tokens.push_back(Token(TokenType::Dedent, "DEDENT", this->position, this->position));
        this->indent_stack.pop();
    }

    tokens.push_back(Token(TokenType::EndOfFile, "EOF", start, this->position));
    return LexerResult(tokens, Error("NULL", "", 0, this->position, this->position));
}