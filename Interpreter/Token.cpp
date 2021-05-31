#include "Token.h"


namespace interpreter
{
	Terminal Token::GetTerminal()
	{
	    return terminal;
	}

	std::string Token::GetValue()
	{
	    return value;
	}

	
}