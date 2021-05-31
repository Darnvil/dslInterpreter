#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "Terminal.h"

namespace interpreter
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

        bool operator<(const Token& rhs) const noexcept
        {
            return this->value < rhs.value;
        }

        //bool operator==(const Token& token);
    	
        Terminal GetTerminal();
        std::string GetValue();
    };

	
}
#endif

