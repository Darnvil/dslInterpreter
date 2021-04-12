#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <map>
#include "Token.h"
#include "VarTable.h"
#include "Lexer.h"

namespace interpreter
{
	class Parser
	{
		
		std::vector<Token>::iterator currentToken;
		std::vector<Token>::iterator endToken;
		int line;

		VarTable top;
		
		std::map<std::string, std::vector<std::string> > firstSets = {
			{"expr", {"TYPE", "VAR", "IF_KW", "FOR_KW", "$"}},
			{"value_expr", {"L_BR", "VAR", "NUMBER", "$"}}
		};

		void lang();
		void expr();
		void var_declaration();
		void assign(bool semicol = true);
		void value_expr();
		void value_expr_wbr();
		void for_expr();
		void for_head();
		void for_init();
		void for_body();
		void if_expr();
		void if_head();
		void if_condition();
		void if_body();
		void else_expr();
		void else_head();
		void else_body();
		void value();
		void logical_expr();
		void checkExpr();
		
		
		void IgnoreWhitespaces();
		void Match(std::string t);
		void err(const std::string& message);

		Lexer lex;

	public:
		Parser()
		{
			line = 0;
			//currentToken = lexer.tokens.begin();
			//endToken = lexer.tokens.end();
			
		}
		
		void Parse(Lexer lexer);
	};
	
}

#endif


