#pragma once
#include "../positions/positions.hpp"
#include <string>

enum struct TokenType {
    EndOfFile,
    Plus,
    Minus,
    Multiply,
    Divide,
    LParen,
    RParen,
    Int,
    Float,
    Identifier,
    Variable,
    Let,
    Constant,
    Is,
    Be,
    Now,
    True,
    False,
    Null,
    Get,
    Module,
    If,
    Unless,
    Elif,
    Else,
    Define,
    With,
    Return,
    Repeat,
    Until,
    Stop,
    Include,
    To,
    Exclude,
    From,
    Element,
    At
};

struct Token {
    TokenType type;
    std::string value;
    Position start;
    Position end;

    Token(const TokenType type, const std::string value, const Position &start, const Position &end);
};