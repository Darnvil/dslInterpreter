#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include "Terminal.h"

namespace Lexer
{
    class Token
    {
    private:
        Terminal terminal;
        std::string value;

    public:
        Token(Terminal t, std::string val) : terminal(t), value(val) {}

        std::string ToString()
        {
            return "[" + terminal.GetIdentifier() + ", " + value + "]";
        }

        Terminal GetTerminal();
        std::string GetValue();
    };
}
#endif

