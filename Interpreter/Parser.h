#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <map>
#include "Token.h"
#include "VarTable.h"
#include "Lexer.h"
#include "Node.h"

namespace interpreter
{
	class Parser
	{
		
		std::vector<Token>::iterator currentToken;
		std::vector<Token>::iterator endToken;
		int line;

		VarTable top;

		Node * node;
		
		std::map<std::string, std::vector<std::string> > firstSets = {
			{"expr", {"TYPE", "VAR", "IF_KW", "FOR_KW", "$"}},
			{"value_expr", {"L_BR", "VAR", "NUMBER", "$"}}
		};

		void lang();
		Node * expr();
		Node * var_declaration();
		Node * assign(bool semicol = true);
		Node * value_expr();
		Node * value_expr_wbr();
		Node * for_expr();
		Node * for_head();
		Node * for_init();
		Node * for_body();
		Node * if_expr();
		Node * if_head();
		Node * if_condition();
		Node * if_body();
		Node * else_expr();
		Node * else_head();
		Node * else_body();
		Node * value();
		Node * logical_expr();

		void checkExpr(Node * parent);
		
		void IgnoreWhitespaces();
		std::string Match(std::string t);
		void err(const std::string& message);

		Lexer lex;

		
	
	public:
		Parser()
		{
			line = 0;	
		}

		~Parser()
		{
			
			
		}
		
		void Parse(Lexer lexer);
	};
	
}

#endif


