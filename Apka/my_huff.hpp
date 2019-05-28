#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
const bool bLeft = false;
const bool bRight = true;

typedef unsigned char Byte;

class huffman_comp {
	Byte setBit(Byte x, unsigned int index);
	bool getBit(Byte x, unsigned int index);
	
	class Node {
	public:
		static bool greaterThan(Node* a, Node* b);
		static bool lessThan(Node* a, Node* b);
		static void removeTree(Node* root);
		Node(Byte b, Node* l = NULL, Node* r = NULL, unsigned long c = 0) : left(l), right(r), count(c), byte(b) {}
		bool isLeaf() const;
		Node* left, *right; //poddrzewa
		unsigned long count; //ilosc wystapien
		Byte byte; //litera w formie unsigned char
	};
	void mapTree(Node * root, std::vector<bool> * codes, std::vector<bool> & prefix);
	void saveTree(Node * root, std::ostream & output, Byte & accumulator, unsigned int& bitIndex);
	bool loadTree(std::istream & input, Byte & accumulator, unsigned int& bitIndex, Node * &root);
public:
	void compress(std::istream& input, std::ostream& output);
	bool decompress(std::istream & input, std::ostream & output);
};