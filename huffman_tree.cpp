#include "huffman_tree.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
#include <string>
#include <queue>
using namespace std;


void codeMap(Node* node, string code, map<char, string>* codes){
	if (node->isLeaf()) {
		if (code == "") {
			code = "0";
		}
		codes->insert({ node->chr, code });
	}
	else {
		string code1 = code + "0";
		string code2 = code + "1";
		codeMap(node->left, code1, codes);
		codeMap(node->right, code2, codes);
	}
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name){
	char chr;
	map<char, int> freqs;
	fstream inFile(file_name, fstream::in);

	while (inFile >> noskipws >> chr) {
		freqs[chr]++;
	}
	inFile.close();

	priority_queue<Node*, vector<Node*>, Compare> pq;
	
	for (pair<char, int> element : freqs) {
		Node* tempNode = new Node;
		tempNode->chr = element.first;
		tempNode->freq = element.second;
		pq.push(tempNode);
	}

	while (pq.size() > 1) {
		Node* temp1 = pq.top();
		pq.pop();
		Node* temp2 = pq.top();
		pq.pop();

		Node* newTree = new Node;
		newTree->chr = NULL;
		newTree->freq = temp1->freq + temp2->freq;
		newTree->left = temp1;
		newTree->right = temp2;
		pq.push(newTree);
	}
	
	this->root = pq.top();
	codesPointer = &codes;
	codeMap(this->root, "", codesPointer);
}

huffman_tree::~huffman_tree(){

}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
	
	for (pair<char, string> elem : codes) {
		if (elem.first == character) {
			return elem.second;
		}
	}
	return "";
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {
	char chr;
	string encoded = "";
	bool valid;
	fstream inFile(file_name, fstream::in);

	while (inFile >> noskipws >> chr) {
		valid = false;
		for (pair<char, string> elem : codes) {
			if (elem.first == chr) {
				encoded += elem.second;
				valid = true;
			}
		}
		if (valid == false) {
			return "";
		}
	}

	inFile.close();

	return encoded;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {
	Node* tempNode = new Node;
	tempNode = this->root;
	string str = string_to_decode;
	string decodedString = "";
	int strLength = str.length();

	
	if (this->root->isLeaf()) {
		decodedString += this->root->chr;
		return decodedString;
	}
	if (str[0] != '0' && str[0] != '1') {
		return "";
	}

	for (int i = 0; i <= strLength; i++) {
		if (tempNode->isLeaf()) {
			decodedString += tempNode->chr;
			tempNode = this->root;
			i--;
		}
		else if (str[i] == '1') {
			tempNode = tempNode->right;
		}
		else if (str[i] == '0') {
			tempNode = tempNode->left;
		}
	}

	return decodedString;
}
