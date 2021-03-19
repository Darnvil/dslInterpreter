#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "Terminal.h"
#include "Token.h"

namespace Lexer
{
	class Lexer
	{
	public:
		const std::vector<Terminal> TERMINALS = {
			Terminal("VAR", "[a-zA-Z]+"),
			Terminal("ASSIGN_OP", "="),
			Terminal("NUMBER", "0|[1-9][0-9]*"),
			Terminal("IF_KW", "if", 1),
			Terminal("ELSE_KW", "else", 1),
			Terminal("FOR_KW", "for", 1),
			Terminal("OP", "\\+|\\-|\\*|\\/"),
			Terminal("L_BR", "\\("),
			Terminal("R_BR", "\\)"),
			Terminal("L_S_BR", "\\{"),
			Terminal("R_S_BR", "\\}"),
			Terminal("LOGICAL_OP", ">|<|<=|>=|!=*|==", 2),
			Terminal("SEMICOLON", ";")
};
		
		
		void Run(std::vector<std::string> args);
		void RunFile(std::ifstream& file);
	private:
		Token ExtractNextToken(std::string input);
		bool AnyTerminalMatches(std::string str);
		std::vector<Terminal> LookupTerminals(std::string str);
		Terminal GetPrioritizedTerminal(std::vector<Terminal> terminals);

		void PrintTokens(std::vector<Token> tokens);

		int line;

	};
}
#endif