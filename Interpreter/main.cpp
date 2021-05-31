#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "StackMachine.h"


int main(int argc, char * argv[])
{
	if (argc == 1)
	{
		std::cout << "No input" << std::endl;
		return -1;
	}

	std::vector<std::string> arguments(argv, argv + argc);
	interpreter::Lexer lexer;
	
	if (arguments[1] == "-f")
	{
		std::ifstream code;
		code.open(arguments[2]);
		if (!code.is_open())
		{
			std::cout << "Can't open file " << arguments[2] << "!\n";
			return -10;
		}

		lexer.RunFile(code);
	}
	else
	{
		lexer.Run(arguments);
	}

	interpreter::Parser parser;
	parser.Parse(lexer);

	interpreter::StackMachine vm(&parser);

	vm.Evaluate();
	
	return 0;
}
