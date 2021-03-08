#include "Terminal.h"

std::string Lexer::Terminal::GetIdentifier()
{
	return identifier;
}

int Lexer::Terminal::GetPriority()
{
	return priority;
}

bool Lexer::Terminal::Matches(std::string str)
{
	return std::regex_match(str, pattern);
}
