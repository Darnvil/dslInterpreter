// ReSharper disable CppClangTidyClangDiagnosticComment
#include "Parser.h"
#include <string>

namespace interpreter
{

	/*
		lang -> expr+
		expr -> assign_expr | if_expr | var_declaration | for_expr
		var_declaration -> TYPE ((VAR SEMICOLON) | assign_expr))
		assign_expr -> VAR ASSIGN_OP value_expr SEMICOLON
		value_expr -> (value_expr_wbr | value) (OP value_expr)*
		value_expr_wbr -> L_BR value_expr R_BR
		value -> NUMBER | VAR
		if_expr -> if_head if_body (else_head else_body)?
		if_head -> IF_KW if_condition
		else_head -> ELSE_KW
		if_condition -> L_BR logical_expr R_BR
		logical_expr -> value_expr LOGICAL_OP value_expr
		if_body -> L_S_BR expr+ R_S_BR
		else_body -> L_S_BR expr+ R_S_BR
		for_head -> FOR_KW L_BR for_init R_BR
		for_init -> (var_declaration | value_expr)? SEMICOLON logical_expr? SEMICOLON value_expr?
		for_body -> L_S_BR expr+ R_S_BR



		VAR -> [a-zA-Z][a-zA-Z1-9_]*
		ASSIGN_OP -> =
		TYPE -> int|float
		IF_KW -> if
		ELSE_KW -> else
		FOR_KW -> for
		L_BR -> (
		R_BR -> )
		L_S_BR -> {
		R_S_BR -> }
		NUMBER -> [0-9]*(.[0-9]*)?(E[\+\-][0-9]*)?
		OP -> [+-/*]
		LOGICAL_OP -> > | < | == | >= | <=
		SEMICOLON -> ;
		NLINE -> \n
		WS -> [\t\s]+
	 */

	
	void Parser::lang()
	{
		checkExpr();
		if(currentToken->GetTerminal().GetIdentifier() != "EOF")
		{
			err("Expected end of file, but met: " + currentToken->GetTerminal().GetIdentifier());
		}
	}

	void Parser::expr()
	{
		auto firstSet = firstSets.at("expr");
		const std::string id = *std::find(firstSet.begin(), firstSet.end(), currentToken->GetTerminal().GetIdentifier());
		if (id == "TYPE")
		{
			var_declaration();
		}
		else if (id == "VAR")
		{
			assign();
		}
		else if (id == "IF_KW")
		{
			if_expr();
		}
		else if (id == "FOR_KW")
		{
			for_expr();
		}
		else
		{
			err("Unexpected token: " + currentToken->GetTerminal().GetIdentifier());
		}
		
	}

	void Parser::assign(bool semicol)
	{
		Match("VAR");
		Match("ASSIGN_OP");
		value_expr();
		if(semicol)
			Match("SEMICOLON");
	}

	void Parser::value_expr()
	{
		auto firstSet = firstSets.at("value_expr");
		IgnoreWhitespaces();
		const std::string id = *std::find(firstSet.begin(), firstSet.end(), currentToken->GetTerminal().GetIdentifier());
		if(id == "L_BR")
		{
			value_expr_wbr();
		}
		else if(id == "VAR" || id == "NUMBER")
		{
			value();
		}
		else
		{
			err("Unexpected token: " + currentToken->GetTerminal().GetIdentifier());
		}
		if (currentToken->GetTerminal().GetIdentifier() == "OP")
		{
			Match("OP");
			value_expr();
		}
		
	}

	void Parser::value_expr_wbr()
	{
		Match("L_BR");
		value_expr();
		Match("R_BR");
		
	}

	void Parser::value()
	{
		try
		{
			Match("NUMBER");
		}
		catch (std::exception e)
		{
			
			Match(("VAR"));
		}
	}

	
	void Parser::var_declaration()
	{
		Match("TYPE");
		if((currentToken + 2)->GetTerminal().GetIdentifier() == "ASSIGN_OP")
		{
			assign();
		}
		else
		{
			Match("VAR");
			Match("SEMICOLON");
		}
	}

	void Parser::for_expr()
	{
		for_head();
		for_body();
	}

	void Parser::for_head()
	{
		Match("FOR_KW");
		Match("L_BR");
		for_init();
		Match("R_BR");
	}

	void Parser::for_init()
	{
		if (currentToken->GetTerminal().GetIdentifier() == "SEMICOLON")
		{
			Match("SEMICOLON");
		}
		else if(currentToken->GetTerminal().GetIdentifier() == "TYPE")
		{
			var_declaration();
		}
		else
		{
			value_expr();
		}
		

		if(currentToken->GetTerminal().GetIdentifier() == "SEMICOLON")
		{
			
		}
		else
		{
			logical_expr();
		}
		Match("SEMICOLON");

		if(currentToken->GetTerminal().GetIdentifier() == "R_BR")
		{
			return;
		}
		assign(false);
		
	}

	void Parser::for_body()
	{
		Match("L_S_BR");
		checkExpr();
		Match("R_S_BR");
	}

	void Parser::if_expr()
	{
		if_head();
		if_body();
		if (currentToken->GetTerminal().GetIdentifier() == "ELSE_KW")
		{
			else_expr();
		}
	}

	
	void Parser::if_head()
	{
		Match("IF_KW");
		if_condition();
	}

	void Parser::if_condition()
	{
		Match("L_BR");
		logical_expr();
		Match("R_BR");
	}

	void Parser::if_body()
	{
		Match("L_S_BR");
		checkExpr();
		Match("R_S_BR");
	}

	void Parser::else_expr()
	{
		else_head();
		else_body();
	}

	void Parser::else_head()
	{
		Match("ELSE_KW");
	}

	void Parser::else_body()
	{
		Match("L_S_BR");
		checkExpr();
		Match("R_S_BR");
	}

	void Parser::logical_expr()
	{
		value_expr();
		Match("LOGICAL_OP");
		value_expr();
	}



	void Parser::checkExpr()
	{
		std::vector<std::string> firstSet = firstSets.at("expr");
		do
		{
			expr();
		} while (std::find(firstSet.begin(), firstSet.end(), currentToken->GetTerminal().GetIdentifier()) != firstSet.end());
	}
	
	void Parser::Match(std::string t)
	{
		IgnoreWhitespaces();
		
		if((currentToken++)->GetTerminal().GetIdentifier() == t)
		{
			IgnoreWhitespaces();
			return;
		}
		--currentToken;
		err("Unexpected token: " + currentToken->GetTerminal().GetIdentifier());
	}

	void Parser::IgnoreWhitespaces()
	{
		while (currentToken->GetTerminal().GetIdentifier() == "WS") ++currentToken;
		while (currentToken->GetTerminal().GetIdentifier() == "NLINE")
		{
			++currentToken;
			++line;
		}
	}

	void Parser::err(const std::string &message)
	{
		throw std::runtime_error(message ); //+ " at line " + std::to_string(line)
	}

	
	void Parser::Parse(Lexer lexer)
	{
		try
		{
			currentToken = lexer.tokens.begin();
			endToken = lexer.tokens.end();
			lang();
			std::cout << "Parser: success\n";
		}
		catch (std::exception exc)
		{
			std::cout << exc.what() << std::endl;
			exit(-2);
		}
	}


	
	
	

	
}

