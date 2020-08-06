#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "AST.h"
#include "Debug.hpp"

#include "Scanner.h"
#include "Parser.h"

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
    for(auto& token : tokens)
        std::cout << token << std::endl;

    Parser parser(tokens);
    std::unique_ptr<Expr> root = parser.parse();

    AstDebugger astDebugger(root.get());
    astDebugger.debug();

}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
		std::cout << "[ERROR] Invalid command line argument count." << std::endl;
		return 1;
    }

    run(argv[1]);
    return 0;
}
