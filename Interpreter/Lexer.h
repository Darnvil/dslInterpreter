#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <iostream>
#include "Terminal.h"
#include "Token.h"

namespace Lexer
{
	class Lexer
	{
	public:
		static const std::vector<Terminal> TERMINALS;

		static Lexer* GetInstance();
		static void Run(std::vector<std::string> args);

	private:
		static Token ExtractNextToken(std::string input);
		static bool AnyTerminalMatches(std::string str);
		static std::vector<Terminal> LookupTerminals(std::string str);
		static Terminal GetPrioritizedTerminal(std::vector<Terminal> terminals);

		static void PrintTokens(std::vector<Token> tokens);

		static Lexer * lexer;
	};
}
#endif