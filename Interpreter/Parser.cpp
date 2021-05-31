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
		Token tok = Token(Terminal("lang", ""), "lang");
		node = new Node(0, "lang");
		
		checkExpr(node);
		node->concatChildsRPN();
		if(currentToken->GetTerminal().GetIdentifier() != "EOF")
		{
			err("Expected end of file, but met: " + currentToken->GetTerminal().GetIdentifier());
		}
	}

	Node * Parser::expr()
	{
		Node * exprn = new Node(line, "expr");
		auto firstSet = firstSets.at("expr");
		const std::string id = *std::find(firstSet.begin(), firstSet.end(), currentToken->GetTerminal().GetIdentifier());
		if (id == "TYPE")
		{
			exprn->AddChild(var_declaration());
		}
		else if (id == "VAR")
		{
			exprn->AddChild(assign());
		}
		else if (id == "IF_KW")
		{
			exprn->AddChild(if_expr());
		}
		else if (id == "FOR_KW")
		{
			exprn->AddChild(for_expr());
		}
		else
		{
			err("Unexpected token: " + currentToken->GetTerminal().GetIdentifier());
		}
		exprn->concatChildsRPN();
		return exprn;
	}

	Node * Parser::assign(bool semicol)
	{
		std::vector<Token> buffer;
		beginExprToken = currentToken;

		tokenCount += 1;
		
		Node * assignn = new Node(line, "assign");
		Node* var = new Leaf(line, Match("VAR"), currentToken->GetValue());
		
		if((currentToken - 2)->GetValue() == "int")
		{
			table.AddVar(*(currentToken - 1), Var<int> {*(currentToken - 1)}); // Add a var to a varTable
		}
		
		assignn->AddChild((Node*)var);
		assignn->RPN.push_back(*(currentToken - 1));
		Node* assign_op = new Leaf(line, Match("ASSIGN_OP"), currentToken->GetValue());
		opStack.push(*(currentToken - 1));
		assignn->AddChild((Node*)assign_op);

		Node * value_expr_node = value_expr();
		assignn->AddChild(value_expr_node);
		assignn->concatChildsRPN();
		
		
		if (semicol)
		{
			Leaf* semicolon = new Leaf(line, Match("SEMICOLON"), currentToken->GetValue());
			assignn->AddChild((Node *)semicolon);
		}

		assignn->RPN.push_back(opStack.top());
		opStack.pop();
		
		return assignn;
	}

	Node * Parser::value_expr(bool internal)
	{
		if(beginExprToken == endToken)
		{
			beginExprToken = currentToken;
		}
		
		Node* value_exprn = new Node(line, "value_expr");
		
		auto firstSet = firstSets.at("value_expr");
		//IgnoreWhitespaces();
		const std::string id = *std::find(firstSet.begin(), firstSet.end(), currentToken->GetTerminal().GetIdentifier());
		
		if(id == "L_BR")
		{
			value_exprn->AddChild(value_expr_wbr());
			value_exprn->concatChildsRPN();
		}
		else if(id == "VAR" || id == "NUMBER")
		{
			value_exprn->AddChild(value());
			value_exprn->concatChildsRPN();
		}
		else
		{
			err("Unexpected token: " + currentToken->GetTerminal().GetIdentifier());
		}
		if (currentToken->GetTerminal().GetIdentifier() == "OP")
		{
			Leaf* op = new Leaf(line, Match("OP"), currentToken->GetValue());
			value_exprn->AddChild((Node *)op);
			opStack.push(*(currentToken - 1));
			value_exprn->AddChild(value_expr());
			value_exprn->concatChildsRPN();
			value_exprn->RPN.push_back(opStack.top());
			opStack.pop();
		}
		
			
		return value_exprn;
	}

	Node * Parser::value_expr_wbr(bool internal)
	{
		Node * val_expr_wbr = new Node(line, "value_expr_wbr");
		Leaf* l_br = new Leaf(line, Match("L_BR"), currentToken->GetValue());
		val_expr_wbr->AddChild((Node *)l_br);
		opStack.push(*(currentToken - 1));
		val_expr_wbr->AddChild(value_expr());
		val_expr_wbr->concatChildsRPN();
		Leaf* r_br = new Leaf(line, Match("R_BR"), currentToken->GetValue());
		val_expr_wbr->AddChild((Node *)r_br);
		while(true)
		{
			if(opStack.top().GetTerminal().GetIdentifier() == "L_BR")
			{
				opStack.pop();
				break;
			}
			val_expr_wbr->RPN.push_back(opStack.top());
			opStack.pop();
		}
		return val_expr_wbr;		
	}

	Node * Parser::value()
	{
		Node * value = new Node(line, "value");
		try
		{
			Leaf* number = new Leaf(line, Match("NUMBER"), currentToken->GetValue());
			value->AddChild((Node *)number);
			value->RPN.push_back(*(currentToken - 1));
			return value;
		}
		catch (std::exception e)
		{
			Leaf* var = new Leaf(line, Match("VAR"), currentToken->GetValue());
			value->AddChild((Node *)var);
			value->RPN.push_back(*(currentToken - 1));
			return value;
		}
	}

	
	Node * Parser::var_declaration()
	{
		Node * var_declarationn = new Node(line, "var_declaration");
		Leaf* type = new Leaf(line, Match("TYPE"), currentToken->GetValue());
		var_declarationn->AddChild((Node *)type);

		
		

		//IgnoreWhitespaces();
		if((currentToken + 1)->GetTerminal().GetIdentifier() == "ASSIGN_OP")
		{
			var_declarationn->AddChild(assign());
			var_declarationn->concatChildsRPN();
			
		}
		else
		{
			Leaf * var = new Leaf(line, Match("VAR"), currentToken->GetValue());
			var_declarationn->AddChild((Node *)var);
			if((currentToken - 2)->GetValue() == "int")
			{
				table.AddVar(*(currentToken - 1), Var<int> {*(currentToken - 1)}); // Add a var to a varTable
			}
			var_declarationn->RPN.push_back(*(currentToken - 1));
			Leaf * semicolon = new Leaf(line, Match("SEMICOLON"), currentToken->GetValue());
			var_declarationn->AddChild((Node *)semicolon);
		}
		return var_declarationn;
	}

	Node * Parser::for_expr()
	{
		Node * for_exprn = new Node(line, "for_expr");
		
		for_exprn->AddChild(for_head());
		for_exprn->for_modifier_buffer = for_exprn->getChilds()[0]->for_modifier_buffer;
		
		for_exprn->AddChild(for_body());
		for_exprn->concatChildsRPN();
		if(for_exprn->for_modifier_buffer != nullptr)
			for_exprn->AddToRpn(for_exprn->for_modifier_buffer);
		for_exprn->RPN.push_back(Token(Terminal("FOR_GOTO_COND", ""), "COND" + std::to_string(COND_ID++) + " !"));
		for_exprn->RPN.push_back(Token(Terminal("FOR_GOTO_END", ""), "END" + std::to_string(END_ID++)));
		
		
		return for_exprn;
	}

	Node * Parser::for_head()
	{
		Node * for_headn = new Node(line, "for_head");
		
		Leaf* for_kw = new Leaf(line, Match("FOR_KW"), currentToken->GetValue());
		for_headn->AddChild((Node *)for_kw);
		
		Leaf* l_br = new Leaf(line, Match("L_BR"), currentToken->GetValue());
		for_headn->AddChild((Node *)l_br);
		
		for_headn->AddChild(for_init());
		for_headn->for_modifier_buffer = for_headn->getChilds()[2]->for_modifier_buffer;
		for_headn->concatChildsRPN();
		for_headn->RPN.push_back(Token(Terminal("FOR_GOTO", ""), "END" + std::to_string(END_ID) + " !F"));
		
		
		Leaf* r_br = new Leaf(line, Match("R_BR"), currentToken->GetValue());
		for_headn->AddChild((Node *)r_br);
		
		return for_headn;
	}

	Node * Parser::for_init()
	{
		Node * for_initn = new Node(line, "for_init");
		if (currentToken->GetTerminal().GetIdentifier() == "SEMICOLON")
		{
			Leaf* semicolon = new Leaf(line, Match("SEMICOLON"), currentToken->GetValue());
			for_initn->AddChild((Node *)semicolon);
		}
		else if(currentToken->GetTerminal().GetIdentifier() == "TYPE")
		{
			for_initn->AddChild(var_declaration());
		}
		else
		{
			for_initn->AddChild(value_expr());
		}

		for_initn->concatChildsRPN();

		for_initn->RPN.push_back(Token(Terminal("FOR_COND_END", ""), "COND" + std::to_string(COND_ID)));
		
		if(currentToken->GetTerminal().GetIdentifier() != "SEMICOLON")
		{
			auto logical_exprn = logical_expr();
			for_initn->AddChild(logical_exprn);
			for_initn->concatChildsRPN();
			
			
		}
		Leaf* semicolon = new Leaf(line, Match("SEMICOLON"), currentToken->GetValue());
		for_initn->AddChild((Node *)semicolon);

		if(currentToken->GetTerminal().GetIdentifier() == "R_BR")
		{
			return for_initn;
		}
		auto assignn = assign(false);
		for_initn->AddChild(assignn);
		for_initn->for_modifier_buffer = assignn;
		
		return for_initn;
	}

	Node * Parser::for_body()
	{
		Node * for_bodyn = new Node(line, "for_body");
		
		Leaf* l_s_br = new Leaf(line, Match("L_S_BR"), currentToken->GetValue());
		for_bodyn->AddChild((Node *)l_s_br);
		
		checkExpr(for_bodyn);
		for_bodyn->concatChildsRPN();
		
		Leaf* r_s_br = new Leaf(line, Match("R_S_BR"), currentToken->GetValue());
		for_bodyn->AddChild((Node *)r_s_br);
		
		
		return for_bodyn;
	}

	Node * Parser::if_expr()
	{
		Node * if_exprn = new Node(line, "if_expr");
		
		if_exprn->AddChild(if_head());
		if_exprn->concatChildsRPN();
		if_exprn->RPN.push_back(Token(Terminal("IF_GOTO", ""), "END" + std::to_string(END_ID) + " !F"));
		
		if_exprn->AddChild(if_body());
		if_exprn->concatChildsRPN();

		
		
		
		
		if (currentToken->GetTerminal().GetIdentifier() == "ELSE_KW")
		{
			if_exprn->RPN.push_back(Token(Terminal("ELSE_GOTO", ""), "GO" + std::to_string(GO_ID) + " !"));
			if_exprn->RPN.push_back(Token(Terminal("IF_GOTO_END", ""), "END" + std::to_string(END_ID++)));
			if_exprn->AddChild(else_expr());
			if_exprn->concatChildsRPN();
		}
		else
		{
			if_exprn->RPN.push_back(Token(Terminal("IF_GOTO_END", ""), "END" + std::to_string(END_ID++)));
		}
		
		return if_exprn;
	}

	
	Node * Parser::if_head()
	{
		Node * if_headn = new Node(line, "if_head");
	
		Leaf* if_kw = new Leaf(line, Match("IF_KW"), currentToken->GetValue());
		if_headn->AddChild((Node *)if_kw);
		
		if_headn->AddChild(if_condition());
		if_headn->concatChildsRPN();
		
		return if_headn;
	}

	Node * Parser::if_condition()
	{
		Node * if_conditionn = new Node(line, "if_condition");
		
		Leaf* l_br = new Leaf(line, Match("L_BR"), currentToken->GetValue());
		if_conditionn->AddChild((Node *)l_br);
		
		if_conditionn->AddChild(logical_expr());
		if_conditionn->concatChildsRPN();
		
		Leaf* r_br = new Leaf(line, Match("R_BR"), currentToken->GetValue());
		if_conditionn->AddChild((Node *)r_br);

		return if_conditionn;
	}

	Node * Parser::if_body()
	{
		Node * if_bodyn = new Node(line, "if_body");
		
		Leaf* l_s_br = new Leaf(line, Match("L_S_BR"), currentToken->GetValue());
		if_bodyn->AddChild((Node *)l_s_br);
		
		checkExpr(if_bodyn);
		if_bodyn->concatChildsRPN();
		
		Leaf* r_s_br = new Leaf(line, Match("R_S_BR"), currentToken->GetValue());
		if_bodyn->AddChild((Node *)r_s_br);

		
		
		return if_bodyn;
	}

	Node * Parser::else_expr()
	{
		Node * else_exprn = new Node(line, "else_expr");
		
		else_exprn->AddChild(else_head());
		else_exprn->AddChild(else_body());

		else_exprn->concatChildsRPN();

		else_exprn->RPN.push_back(Token(Terminal("ELSE_GOTO_END", ""), "GO" + std::to_string(GO_ID++)));
		
		return else_exprn;
	}

	Node * Parser::else_head()
	{
		Node * else_headn = new Node(line, "else_head");
		
		Leaf* else_kw = new Leaf(line, Match("ELSE_KW"), currentToken->GetValue());
		else_headn->AddChild((Node *)else_kw);
		
		return else_headn;
	}

	Node * Parser::else_body()
	{
		Node * else_bodyn = new Node(line, "else_body");
		
		Leaf* l_s_br = new Leaf(line, Match("L_S_BR"), currentToken->GetValue());
		else_bodyn->AddChild((Node *)l_s_br);
		
		checkExpr(else_bodyn);
		else_bodyn->concatChildsRPN();
		
		Leaf* r_s_br = new Leaf(line, Match("R_S_BR"), currentToken->GetValue());
		else_bodyn->AddChild((Node *)r_s_br);

		return else_bodyn;
	}

	Node * Parser::logical_expr()
	{
		Node * logical_exprn = new Node(line, "logical_expr");
		
		logical_exprn->AddChild(value_expr());
		logical_exprn->concatChildsRPN();
		
		Leaf* logical_op = new Leaf(line, Match("LOGICAL_OP"), currentToken->GetValue());
		logical_exprn->AddChild((Node *)logical_op);
		opStack.push(*(currentToken - 1));
		
		logical_exprn->AddChild(value_expr());
		logical_exprn->concatChildsRPN();

		logical_exprn->RPN.push_back(opStack.top());
		opStack.pop();
		
		return logical_exprn;
	}



	void Parser::checkExpr(Node * parent)
	{
		std::vector<std::string> firstSet = firstSets.at("expr");
		do
		{
			
			parent->AddChild(expr());
			
		} while (std::find(firstSet.begin(), firstSet.end(), currentToken->GetTerminal().GetIdentifier()) != firstSet.end());
	}

	
	void Parser::ToOutputRPN()
	{
		std::vector<Token> outputLine;
		
		
	}
	
	std::string Parser::Match(std::string t)
	{
		//IgnoreWhitespaces();
		
		if((currentToken++)->GetTerminal().GetIdentifier() == t)
		{
			//IgnoreWhitespaces();
			return t;
		}
		--currentToken;
		err("Unexpected token: " + currentToken->GetTerminal().GetIdentifier());
	}

	std::vector<Token> Parser::IgnoreWhitespaces()
	{
		auto it = currentToken;
		std::vector<Token> newTokens;
		for(; it != endToken; ++it)
		{
			if (it->GetTerminal().GetIdentifier() != "WS")
			{
				newTokens.push_back(*it);
			}
		}

		return newTokens;
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

			auto tokens = IgnoreWhitespaces();

			currentToken = tokens.begin();
			endToken = tokens.end();
			
			lang();
			std::cout << "Parser: success\n";
			node->Print(node, 0);

			std::cout << "\nRPN: \n\n";

			node->PrintRPN();
		}
		catch (std::exception exc)
		{
			std::cout << exc.what() << std::endl;
			exit(-2);
		}
	}
}