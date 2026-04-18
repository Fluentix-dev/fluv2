#include "tokens.hpp"

Token::Token(const TokenType type, const std::string value, const Position &start, const Position &end) : type(type), value(value), start(start), end(end) {}