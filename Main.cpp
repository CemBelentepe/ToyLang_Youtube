#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "AST.h"
#include "Debug.hpp"
#include "Interpreter.h"
#include "Parser.h"
#include "Scanner.h"

void run(const char* filePath)
{
    std::ifstream file(filePath);
    if (file.fail())
    {
        std::cout << "[ERROR] Unable to find file with path: " << filePath << std::endl;
        return;
    }

    std::stringstream sourceStream;
    sourceStream << file.rdbuf();
    std::string source = sourceStream.str();

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    // for (auto& token : tokens)
    //     std::cout << token << std::endl;

    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> root = parser.parse();

    if (parser.fail())
    {
        std::cout << "Exited due to parse error" << std::endl;
        return;
    }

    AstDebugger astDebugger(root);
    astDebugger.debug();

    Interpreter interpreter(root);
    int runError = interpreter.run();
    if (runError != 0)
    {
        std::cout << "Exited with error code " << runError << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "[ERROR] Invalid command line argument count." << std::endl;
        return 1;
    }

    run(argv[1]);
    return 0;
}
