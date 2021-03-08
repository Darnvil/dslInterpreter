#include "Token.h"


Lexer::Terminal Lexer::Token::GetTerminal()
{
    return terminal;
}

std::string Lexer::Token::GetValue()
{
    return value;
}