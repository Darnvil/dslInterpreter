#include "StackMachine.h"

namespace interpreter
{
	void StackMachine::Evaluate()
	{
		currentToken = RPN.begin();
		endToken = RPN.end();
		
		while(currentToken != endToken)
		{
			auto id = currentToken->GetTerminal().GetIdentifier();
			auto value = currentToken->GetValue();
			if(id == "OP")
			{
				auto op1 = operandStack.back();
				operandStack.pop_back();
				auto op2 = operandStack.back();
				operandStack.pop_back();

				int val1;
				int val2;
				
				if (op1.GetTerminal().GetIdentifier() == "VAR")
				{
					auto *var = table->SeekIntVar(op1);
					val1 = var->value;
				}
				else
				{
					val1 = std::stoi(op1.GetValue());
				}

				if (op2.GetTerminal().GetIdentifier() == "VAR")
				{
					auto *var = table->SeekIntVar(op2);
					val2 = var->value;
				}
				else
				{
					val2 = std::stoi(op2.GetValue());
				}

				int res;
				
				if (value == "+")
				{
					res = add(val2, val1);
				}
				if (value == "-")
				{
					res = sub(val2, val1);
				}
				if (value == "*")
				{
					res = mul(val2, val1);
				}
				if (value == "/")
				{
					res = div(val2, val1);
				}

				auto resToken = Token(Terminal("INT", ""), std::to_string(res));
				operandStack.push_back(resToken);
			}
			else if (id == "ASSIGN_OP")
			{
				auto op1 = operandStack.back();
				operandStack.pop_back();
				auto op2 = operandStack.back();
				operandStack.pop_back();

				auto *var1 = table->SeekIntVar(op2);

				if(op1.GetTerminal().GetIdentifier() == "VAR")
				{
					auto * var2 = table->SeekIntVar(op1);
					auto val = var2->value;
					assign(var1, val);
				}
				else
				{
					auto val = std::stoi(op1.GetValue());
					assign(var1, val);
				}
				
			}
			else if (id == "LOGICAL_OP")
			{
				auto op1 = operandStack.back();
				operandStack.pop_back();
				auto op2 = operandStack.back();
				operandStack.pop_back();

				int val1;
				int val2;
				
				if (op1.GetTerminal().GetIdentifier() == "VAR")
				{
					auto *var = table->SeekIntVar(op1);
					val1 = var->value;
				}
				else
				{
					val1 = std::stoi(op1.GetValue());
				}

				if (op2.GetTerminal().GetIdentifier() == "VAR")
				{
					auto *var = table->SeekIntVar(op2);
					val2 = var->value;
				}
				else
				{
					val2 = std::stoi(op2.GetValue());
				}

				bool res;

				if(value == "<")
				{
					res = LT(val2, val1);
				}
				if(value == ">")
				{
					res = GT(val2, val1);
				}
				if(value == "==")
				{
					res = EQ(val2, val1);
				}
				if(value == "<>")
				{
					res = NEQ(val2, val1);
				}
				if(value == "<=")
				{
					res = LE(val2, val1);
				}
				if(value == ">=")
				{
					res = GE(val2, val1);
				}
				
				auto resToken = Token(Terminal("BOOL", ""), res ? "True" : "False");
				operandStack.push_back(resToken);
				
			}
			else if(id == "FOR_GOTO" || id == "IF_GOTO")
			{
				auto op1 = operandStack.back();
				operandStack.pop_back();

				std::string addr;
				
				while(value.at(0) != ' ')
				{
					addr.push_back(value.at(0));
					value = value.substr(1, value.length() - 1);
				}

				FalseGo(op1, addr);
			
			}
			else if(id == "FOR_GOTO_COND")
			{
				std::string addr;
				
				while(value.at(0) != ' ')
				{
					addr.push_back(value.at(0));
					value = value.substr(1, value.length() - 1);
				}

				GoBack(addr);
			}
			else if (id == "ELSE_GOTO")
			{
				std::string addr;
				
				while(value.at(0) != ' ')
				{
					addr.push_back(value.at(0));
					value = value.substr(1, value.length() - 1);
				}

				Go(addr);
			}
			else if(id == "VAR" || id == "NUMBER")
			{
				operandStack.push_back(*currentToken);
			}

			++currentToken;
		}


		std::cout << "\nVarTable\n\n";

		table->PrintTable();
	}

	/*bool StackMachine::isOp(Token token)
	{
		auto value = token.GetValue();
	}

	bool StackMachine::isLogicalOp(Token token)
	{

	}

	bool StackMachine::isFalseGo(Token token)
	{

	}

	bool StackMachine::isGo(Token token)
	{

	}*/


	int StackMachine::add(int a, int b)
	{
		return a + b;
	}

	int StackMachine::sub(int a, int b)
	{
		return a - b;
	}
	
	int StackMachine::mul(int a, int b)
	{
		return a * b;
	}
	
	int StackMachine::div(int a, int b)
	{
		return a / b;
	}


	void StackMachine::assign(Var<int> * var, int val)
	{
		var->value = val;
	}
	

	bool StackMachine::LT(int a, int b)
	{
		return a < b;
	}
	
	bool StackMachine::GT(int a, int b)
	{
		return a > b;
	}
	
	bool StackMachine::EQ(int a, int b)
	{
		return a == b;
	}
	
	bool StackMachine::NEQ(int a, int b)
	{
		return a != b;
	}
	
	bool StackMachine::LE(int a, int b)
	{
		return a <= b;
	}
	
	bool StackMachine::GE(int a, int b)
	{
		return a >= b;
	}

	
	void StackMachine::FalseGo(Token cond, std::string addrToJump)
	{
		if (cond.GetValue() == "True")
		{
			return;
		}
		while(currentToken->GetValue() != addrToJump)
		{
			++currentToken;
		}
	}
	
	void StackMachine::Go(std::string addrToJump)
	{
		while(currentToken->GetValue() != addrToJump)
		{
			++currentToken;
		}
	}

	void StackMachine::GoBack(std::string addrToJump)
	{
		while(currentToken->GetValue() != addrToJump)
		{
			--currentToken;
		}
	}
	
}
