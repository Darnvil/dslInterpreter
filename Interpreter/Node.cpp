#include "Node.h"

#include <utility>


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

	std::vector<Node*> Node::getChilds()
	{
		return childs;
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

	
	
	void Node::AddToRpn(Node * node)
	{
		for(auto token : node->RPN)
		{
			RPN.push_back(token);
		}
	}

	void Node::concatChildsRPN()
	{
		for(size_t i = currentChildToRpn; i < childs.size(); ++i)
		{
			for (auto tok : childs[i]->RPN)
			{
				RPN.push_back(tok);
			}
			++currentChildToRpn;
		}
	}

	void Node::PrintRPN()
	{
		
		
			/*Leaf * leaf = dynamic_cast<Leaf *>(node);
			if (leaf)
				leaf->PrintNode(node, indent);
			else
				PrintNode(node, indent);
		
		
		
		if (node->RPN.empty())
		{
			std::cout << std::endl;
			
			for (auto child : node->childs)
			{
				PrintRPN(child, indent + 3);
			}
		}
		else
		{
			std::cout << std::endl;

			for (auto child : node->RPN)
			{
				PrintRPN(child, indent + 3);
			}
			
		}*/

		for(auto token : RPN)
		{
			std::cout << token.GetValue() << " ";
		}
			
	}



	void Leaf::PrintNode(Node * node, int indent) 
	{
		Leaf *leaf = dynamic_cast<Leaf*>(node);
		
		for (int i = indent; i > 0; --i) std::cout << " ";
		std::cout << leaf->identifier + " \n";
		for (int i = indent + 9; i > 0; --i) std::cout << " ";
		std::cout << leaf->value;
	}
}