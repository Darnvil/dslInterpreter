#include "Terminal.h"

namespace interpreter
{

	std::string Terminal::GetIdentifier()
	{
		return identifier;
	}

	int Terminal::GetPriority()
	{
		return priority;
	}

	bool Terminal::Matches(std::string str)
	{
		return std::regex_match(str, pattern);
	}
}