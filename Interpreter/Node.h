#pragma once

#include "Token.h"
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>

namespace interpreter
{
	class Leaf;

	class Node
	{
	
		int line;

		

	protected:
		std::vector<Node*> childs;
		
		
	public:
		std::string  identifier;

		Node * for_modifier_buffer;
		
		Node(const int l, const std::string& id)
		{
			line = l;
			identifier = id;
			currentChildToRpn = 0;
		}

		int currentChildToRpn;
		
		void Print(Node * node, const int indent);
		

		void virtual PrintNode(Node * node, int indent);

		void concatChildsRPN();

		;
		void AddChild(Node * node);
		std::vector<Node *> getChilds();

		void AddToRpn(Node * node);

		void PrintRPN();

		std::vector<Token> RPN;
		
	};

	class Leaf : public Node
	{
		std::string value;
	public:
		Leaf(const int l, const std::string& id, std::string val) : Node(l, id) { value = val; }

		void PrintNode(Node * node, int indent) override;
		std::string GetValue()
		{
			return value;
		}

		
	};

	
	
}
