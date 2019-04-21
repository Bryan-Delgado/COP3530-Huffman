#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <map>

class Node {
public:
	int freq;
	char chr;
	int code = 0;
	Node *left = NULL;
	Node *right = NULL;
	bool isLeaf() {
		if (left == NULL && right == NULL) {
			return true;
		}
		else {
			return false;
		}
	}
};


class huffman_tree {
	public:
		huffman_tree(const std::string &file_name);
		~huffman_tree();
		
		std::map<char, std::string>* codesPointer;
		std::map<char, std::string> codes;
		Node* root = new Node;
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;	
};



class Compare {
public:
	bool operator()(Node* const& node1, Node* const& node2) const
	{
		return node1->freq > node2->freq;
	}
};

#endif