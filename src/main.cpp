#include "lexer/lexer.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc == 1) {
        std::cout << "Flu is a language that your mom hates the most...\nType 'flu help' to show all of the commands\n";
        return 0;
    }

    std::string command = argv[1];
    if (command == "help") {
        std::cout << "List of commands:\n[1] flu --version: Shows the version of Flu you are currently using\n[2] flu help: Shows all commands you can use\n[3] flu conv <file_name> (in early development): Transpiles the file to Python code\n";
        return 0;
    }

    if (command == "--version") {
        std::cout << "Flu version Beta - In development...";
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
        if (tokens.error.type == "NULL") {
            for (const Token &token : tokens.tokens) {
                std::cout << static_cast<int>(token.type) << " " << token.value << "\n";
            }
        } else {
            tokens.error.print();
            return 0;
        }

        return 0;
    }
    
    std::cout << "Command '" << command << "' not found";
    return 0;
}