#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>

void print_expr(const std::shared_ptr<Expression> &node) {
    if (node->type == ExpressionType::Binary) {
        std::shared_ptr<BinaryExpression> binary = std::static_pointer_cast<BinaryExpression>(node);
        std::cout << "(";
        print_expr(binary->left);
        std::cout << " " << binary->op << " ";
        print_expr(binary->right);
        std::cout << ")";
        return;
    }

    if (node->type == ExpressionType::Unary) {
        std::shared_ptr<UnaryExpression> unary = std::static_pointer_cast<UnaryExpression>(node);
        std::cout << "(" << unary->op;
        print_expr(unary->value);
        std::cout << ")";
        return; 
    }

    if (node->type == ExpressionType::IntLiteral) {
        std::shared_ptr<IntExpression> literal = std::static_pointer_cast<IntExpression>(node);
        std::cout << literal->literal;
        return;
    }

    if (node->type == ExpressionType::FloatLiteral) {
        std::shared_ptr<FloatExpression> literal = std::static_pointer_cast<FloatExpression>(node);
        std::cout << literal->literal;
        return;
    }

    if (node->type == ExpressionType::Identifier) {
        std::shared_ptr<IdentifierExpression> identifier = std::static_pointer_cast<IdentifierExpression>(node);
        std::cout << identifier->identifier;
        return;
    }

    if (node->type == ExpressionType::Group) {
        std::shared_ptr<GroupExpression> group = std::static_pointer_cast<GroupExpression>(node);
        std::cout << "(";
        print_expr(group->inside);
        std::cout << ")";
        return;
    }
}

void print_stmt(const std::shared_ptr<Statement> &node, const size_t indentation = 0) {
    for (size_t i = 0; i < indentation; i++) {
        std::cout << "    ";
    }

    if (node->type == StatementType::Block) {
        std::shared_ptr<BlockStatement> block = std::static_pointer_cast<BlockStatement>(node);
        std::cout << "BLOCK (len=" << block->body.size() << ")\n";
        for (const std::shared_ptr<Statement> &stmt : block->body) {
            print_stmt(stmt, indentation+1);
            std::cout << "\n";
        }

        return;
    }
    
    if (node->type == StatementType::Expression) {
        std::shared_ptr<ExpressionStatement> expr = std::static_pointer_cast<ExpressionStatement>(node);
        print_expr(expr->expression);
        return;
    }
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc == 1) {
        std::cout << "Flu is a language that your mom hates the most...\nType 'flu help' to show all of the commands\n";
        return 0;
    }

    std::string command = argv[1];
    if (command == "help") {
        std::cout << "List of commands:\n[1] flu --version: Shows the version of Flu you are currently using\n[2] flu help: Shows all commands you can use\n[3] flu conv <file_name> (in early development): Transpiles the file to Python code\n[4] flu clean: Clears the terminal\n";
        return 0;
    }

    if (command == "--version") {
        std::cout << "Flu version Alpha - In development...";
        return 0;
    }

    if (command == "conv") {
        if (argc == 2) {
            std::cout << "File name not provided\n";
            return 0;
        }

        std::string fn = argv[2];
        std::ifstream src(fn);
        if (!src) {
            std::cout << "File " << fn << " cannot be found!\n";
            return 0;
        }

        std::stringstream code;
        std::string line;
        while (src.is_open() && std::getline(src, line)) {
            code << line << "\n";
        }

        std::string real = code.str();
        if (real.size() > 0) {
            real.erase(std::prev(real.end())); // Remove the ending newline
        }

        Lexer lexer = Lexer(fn, real);
        LexerResult tokens = lexer.tokenize();
        if (tokens.error.type != "NULL") {
            tokens.error.print();
            return 0;
        }

        Parser parser = Parser(tokens.tokens);
        StatementResult ast = parser.parse();
        if (ast.error.type != "NULL") {
            ast.error.print();
            return 0;
        }

        print_stmt(ast.node);
        return 0;
    }
 
    if (command == "clean") {
        #ifdef _WIN32
            system("cls");
        #else
            std::cout << "\033[2J\033[H" << std::flush;
        #endif
        return 0;
    }

    std::cout << "Command '" << command << "' not found";
    return 0;
}
