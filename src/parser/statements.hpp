#pragma once
#include "nodes.hpp"
#include <vector>

struct BlockStatement : public Statement {
    std::vector<Statement> body;

    BlockStatement(const std::vector<Statement> &body);
};