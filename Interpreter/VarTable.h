#pragma once

#ifndef VAR_TABLE_H
#define VAR_TABLE_H

#include <map>
#include "Token.h"

namespace interpreter
{
	

	class Var
	{
	public:
		Token token;
		std::string value;
		Token type;

		Var(Token tok, std::string val, Token ty)
			: token(tok), value(val), type(ty)  {}

		Var(Token tok, Token ty)
			: token(tok), value(""), type(ty)  {}
	};
	
	class VarTable
	{
	private:
		VarTable * prev;
		
		std::map < Token, Var > Table;
	public:
		VarTable()
		{
			prev = nullptr;
		}

		VarTable(VarTable * pr)
		{
			prev = pr;
		}

		void AddVar(Token token, Var var);
		Var * SeekVar(Token token);
		void ChangeVal(Token token, std::string val);
		

		
	};

	
	
}


#endif

