#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"


int main(int argc, char * argv[])
{
	if (argc == 1)
	{
		std::cout << "No input" << std::endl;
		return -1;
	}

	std::vector<std::string> arguments(argv, argv + argc);

	Lexer::Lexer::GetInstance()->Run(arguments);
	
	
	return 0;
}