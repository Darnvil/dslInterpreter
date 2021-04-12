#pragma once

#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <string>
#include <regex>

namespace interpreter
{

    class Terminal
    {
    private:
        std::string identifier;
        std::regex pattern;
        int priority;

    public:
        Terminal(std::string id, std::string patt)
        {
            try 
            {
                identifier = id;
                pattern = std::regex(patt);
                priority = 0;
            }
            catch (const std::exception & e)
            {
                std::cout << e.what();
            }
        }

        Terminal(std::string id, std::string patt, int prior)
        {
            try 
            {
                identifier = id;
                pattern = std::regex(patt);
                priority = prior;
            }
            catch (const std::exception & e)
            {
                std::cout << e.what();
            }
        }

        //copy ctor
        Terminal(const Terminal & cpy) : identifier(cpy.identifier), pattern(cpy.pattern), priority(cpy.priority)  {}

        std::string GetIdentifier();
        int GetPriority();
        bool Matches(std::string str);
    };

}
#endif
