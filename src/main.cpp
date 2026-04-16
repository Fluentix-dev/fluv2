#include <iostream>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc == 1) {
        std::cout << "EnglishPy, a language that transpiles English code to Python"
    }
    return 0;
}