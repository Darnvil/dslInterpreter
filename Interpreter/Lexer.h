#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "Terminal.h"
#include "Token.h"

namespace interpreter
{
	class Lexer
	{
	public:
		const std::vector<Terminal> TERMINALS = {
			Terminal("VAR", "[a-zA-Z][a-zA-Z1-9_]*"),
			Terminal("ASSIGN_OP", "="),
			Terminal("NUMBER", R"([0-9]*)"), //(\.[0-9]*)?(E[\+\-][0-9]*)?
			Terminal("TYPE", "int|float", 1),
			Terminal("IF_KW", "if", 1),
			Terminal("ELSE_KW", "else", 1),
			Terminal("FOR_KW", "for", 1),
			Terminal("OP", R"(\+|\-|\*|\/)"),
			Terminal("L_BR", "\\("),
			Terminal("R_BR", "\\)"),
			Terminal("L_S_BR", "\\{"),
			Terminal("R_S_BR", "\\}"),
			Terminal("LOGICAL_OP", ">|<|<=|>=|<>|==", 2),
			Terminal("SEMICOLON", ";"),
			Terminal("WS", "[\\t\\r ]+"),
			Terminal("NLINE", "\\n", 1)
};

		std::vector<Token> tokens;
		
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