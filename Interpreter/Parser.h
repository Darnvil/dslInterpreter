#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <map>
#include <stack>
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

		std::vector<Token>::iterator beginExprToken;
		
		int line;

		
		int tokenCount; // for RPN

		std::vector<std::vector<Token>> outputRPN;
		
		std::stack<Token> opStack;

		

		int pos1;
		int pos2;
		
		VarTable table;

		Node * node;
		
		std::map<std::string, std::vector<std::string> > firstSets = {
			{"expr", {"TYPE", "VAR", "IF_KW", "FOR_KW", "$"}},
			{"value_expr", {"L_BR", "VAR", "NUMBER", "$"}}
		};

		void lang();
		Node * expr();
		Node * var_declaration();
		Node * assign(bool semicol = true);
		Node * value_expr(bool internal = false); 
		Node * value_expr_wbr(bool internal = false);
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

		void ToOutputRPN();
		
		std::vector<Token> IgnoreWhitespaces();
		std::string Match(std::string t);
		void err(const std::string& message);

		Lexer lex;

		int END_ID;
		int COND_ID;
		int GO_ID;
	
	public:
		Parser()
		{
			line = 0;
			tokenCount = 0;
			END_ID = 0;
			COND_ID = 0;
			GO_ID = 0;
		}
		
		void Parse(Lexer lexer);

		VarTable * getTable()
		{
			return &table;
		}

		std::vector<Token> getRPN()
		{
			return node->RPN;
		}
	};
	
}

#endif


