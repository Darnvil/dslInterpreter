#pragma once

#include <vector>
#include "Token.h"
#include "VarTable.h"
#include "Parser.h"

namespace interpreter
{
	class StackMachine
	{
		//Parser * parser;

		VarTable * table;
		std::vector<Token> RPN;

		std::vector<Token>::iterator currentToken;
		std::vector<Token>::iterator endToken;

		std::vector<Token> operandStack;

	public:
		StackMachine(Parser * parser)
		{
			table = parser->getTable();
			RPN = parser->getRPN();
		}

		void Evaluate();

		/*bool isOp(Token token);
		bool isLogicalOp(Token token);
		bool isFalseGo(Token token);
		bool isGo(Token token);*/

		int add(int a, int b);
		int sub(int a, int b);
		int mul(int a, int b);
		int div(int a, int b);

		void assign(Var<int>* var, int val);

		bool LT(int a, int b);
		bool GT(int a, int b);
		bool EQ(int a, int b);
		bool NEQ(int a, int b);
		bool LE(int a, int b);
		bool GE(int a, int b);

		void FalseGo(Token cond, std::string addrToJump);
		void Go(std::string addrToJump);
		void GoBack(std::string addrToJump);
	};
}
