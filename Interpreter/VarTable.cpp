
#include "VarTable.h"

namespace interpreter
{

	void VarTable::AddVar(Token token, Var<int> var)
	{
		if (SeekIntVar(token) == nullptr)
			intTable.insert(std::pair<Token, Var<int> >(token, var));
	}
	
	Var<int>* VarTable::SeekIntVar(Token token)
	{
		Var<int> * found = nullptr;
		if(intTable.find(token) != intTable.end())
			found = &intTable.find(token)->second;
		
		if (found != nullptr)
			return found;
		
		return nullptr;
	}
	
	void VarTable::ChangeIntVal(Token token, int val)
	{
		Var<int> * found = SeekIntVar(token);
		if (found != nullptr)
			found->value = val;
	}

	void VarTable::PrintTable()
	{
		for ( auto intVar : intTable)
		{
			std::cout << intVar.second.token.GetValue() << " = " << intVar.second.value << std::endl;
		}
	}

	
}