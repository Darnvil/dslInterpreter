#include "Node.h"


namespace interpreter
{
	void  Node::PrintNode(Node * node, int indent)
	{
		for (; indent > 0; --indent) std::cout << " ";
		std::cout << node->identifier;
	}

	void Node::AddChild(Node * node)
	{
		childs.push_back(node);
	}

	void Node::Print(Node * node, const int indent)
	{
		Leaf *leaf = dynamic_cast<Leaf*>(node);
		if(leaf)
			leaf->PrintNode(node, indent);
		else
			PrintNode(node, indent);

		
		std::cout << std::endl;

		for (auto child : node->childs)
		{
			Print(child, indent + 3);
		}
	}



	void Leaf::PrintNode(Node * node, int indent) 
	{
		Leaf *leaf = dynamic_cast<Leaf*>(node);
		
		for (int i = indent; i > 0; --i) std::cout << " ";
		std::cout << leaf->identifier + " \n";
		for (int i = indent + 3; i > 0; --i) std::cout << " ";
		std::cout << leaf->value;
	}
}