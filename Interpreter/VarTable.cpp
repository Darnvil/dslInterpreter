
#include "VarTable.h"

namespace interpreter
{

	void VarTable::AddVar(Token token, Var var)
	{
		if (SeekVar(token) == nullptr)
			Table.insert(std::pair<Token, Var>(token, var));
	}
	Var * VarTable::SeekVar(Token token)
	{
		Var * found = &Table.find(token)->second;
		for (VarTable * table = this; table->prev != nullptr; table = table->prev)
		{
			Var * found = &table->Table.find(token)->second;
			if (found != nullptr)
				return found;
		}
		return nullptr;
	}
	void VarTable::ChangeVal(Token token, std::string val)
	{
		Var * found = SeekVar(token);
		if (found != nullptr)
			found->value = val;
	}
	
}