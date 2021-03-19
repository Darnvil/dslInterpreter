#include "Lexer.h"
#include <fstream>



void Lexer::Lexer::Run(std::vector<std::string> args)
{
	line = 0;
	std::vector<Token> tokens;
	for (size_t i = 1; i < args.size(); ++i)
	{
		std::string input = args[i];
		while (input != "")
		{
			while (input.at(0) == ' ' || input.at(0) == '\t' || input.at(0) == '\n')
			{
				input = input.substr(1, input.length());
			}

			Token token = ExtractNextToken(input);
			tokens.push_back(token);

			input = input.substr(token.GetValue().length(), input.length());
		}
	}

	PrintTokens(tokens);
}

void Lexer::Lexer::RunFile(std::ifstream & file)
{
	line = 0;
	std::string buffer;
	std::vector<std::string> lines;
	while (!file.eof())
	{
		std::getline(file, buffer);
		lines.push_back(buffer);
	}
	file.close();

	std::vector<Token> tokens;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		std::string input = lines[i];
		while (input != "")
		{
			while (input.at(0) == ' ' || input.at(0) == '\t' || input.at(0) == '\n')
			{
				if (input.at(0) == '\n')
					line += 1;
				input = input.substr(1, input.length());
			}

			Token token = ExtractNextToken(input);
			tokens.push_back(token);

			input = input.substr(token.GetValue().length(), input.length());
		}
		line++;
	}

	PrintTokens(tokens);

}

Lexer::Token Lexer::Lexer::ExtractNextToken(std::string input)
{
	std::string buffer;
	
	buffer.push_back(input.at(0));
	try {
		if (AnyTerminalMatches(buffer))
		{
			while (AnyTerminalMatches(buffer) && buffer.length() != input.length())
			{
				buffer.push_back(input.at(buffer.length()));
			}

			if(buffer.length() != input.length() || !AnyTerminalMatches(buffer))
				buffer.pop_back();

			std::vector<Terminal> terminals = LookupTerminals(buffer);

			return Token(GetPrioritizedTerminal(terminals), buffer);
		}
		else
		{
			std::string err = "Unexpected symbol at line " + std::to_string(line);
			throw std::runtime_error(err);
		}
	}
	catch(std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
		exit(-1);
	}
}

bool Lexer::Lexer::AnyTerminalMatches(std::string str)
{
	return LookupTerminals(str).size() != 0;
}

std::vector<Lexer::Terminal> Lexer::Lexer::LookupTerminals(std::string str)
{
	std::vector<Terminal> matched;
	for (auto terminal : TERMINALS)
	{
		if (terminal.Matches(str))
		{
			matched.push_back(terminal);
		}
	}
	return matched;
}

Lexer::Terminal Lexer::Lexer::GetPrioritizedTerminal(std::vector<Terminal> terminals)
{
	Terminal prioritized = terminals.at(0);

	for (auto terminal : terminals)
	{
		if (terminal.GetPriority() > prioritized.GetPriority())
		{
			prioritized = terminal;
		}
	}
	return prioritized;
}

void Lexer::Lexer::PrintTokens(std::vector<Token> tokens)
{
	for (auto token : tokens)
	{
		std::cout << token.ToString() << "\n";
	}
}
