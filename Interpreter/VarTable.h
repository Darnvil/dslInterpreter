#pragma once

#ifndef VAR_TABLE_H
#define VAR_TABLE_H

#include <map>
#include "Token.h"

namespace interpreter
{
	
	template <class T>
	class Var
	{
	public:
		Token token;
		T value;
		

		Var(Token tok, T val)
			: token(tok), value(val)   {}

		Var(Token tok)
			: token(tok)   {}
	};
	
	class VarTable
	{
	private:
		
		
		std::map < Token, Var<int> > intTable;
	public:
		
		

		void AddVar(Token token, Var<int> var);
		Var<int> * SeekIntVar(Token token);
		void ChangeIntVal(Token token, int val);
		void PrintTable();

		
	};

	
	
}


#endif

