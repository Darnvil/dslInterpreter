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
		Node(const int l, const std::string& id)
		{
			line = l;
			identifier = id;
			
		}
		~Node()
		{
			/*for (auto child : *childs) delete child;
			delete[] childs;*/
		}
		
		void Print(Node * node, const int indent);
		

		void virtual PrintNode(Node * node, int indent);
		

		void AddChild(Node * node);

		

		
	};

	class Leaf : public Node
	{
		std::string value;
	public:
		Leaf(const int l, const std::string& id, std::string val) : Node(l, id) { value = val; }

		void PrintNode(Node * node, int indent) override;

		
	};

	
	
}
