#include "stdafx.h"
#include "my_huff.hpp"
	
Byte huffman_comp::setBit(Byte x, unsigned int index) {
		return x | (1 << index);
	}

	bool huffman_comp::getBit(Byte x, unsigned int index) {
		return (x & (1 << index)) != 0;
	}

		bool huffman_comp::Node::greaterThan(Node* a, Node* b) {
			if (a != NULL && b != NULL) {
				return a->count > b->count;
			}
			else if (a != NULL) {
				return true;
			}
			else {
				return false;
			}
		}

		bool huffman_comp::Node::lessThan(Node* a, Node* b) {
			if (a != NULL && b != NULL) {
				return a->count < b->count;
			}
			else if (b != NULL) {
				return true;
			}
			else {
				return false;
			}
		}

		void huffman_comp::Node::removeTree(Node* root) {
			if (root == NULL) {
				return;
			}
			removeTree(root->left);
			removeTree(root->right);
			delete root;
		}

		bool huffman_comp::Node::isLeaf() const {
			return this->left == NULL && this->right == NULL;
		}

//////////////////////////////////////////////////////////////////////

	// Wyzncza kod dla każdego liścia w danym drzewie.
	void huffman_comp::mapTree(Node * root, std::vector<bool> * codes, std::vector<bool> & prefix) {
		if (root == NULL) {
			return;
		}

		if (root->left == NULL && root->right == NULL) {
			// Jesteśmy w liściu. Znaleźliśmy kod jednego bajtu.
			codes[root->byte] = prefix;
		}

		if (root->left != NULL) {
			prefix.push_back(bLeft);
			mapTree(root->left, codes, prefix);
			prefix.pop_back();
		}
		if (root->right != NULL) {
			prefix.push_back(bRight);
			mapTree(root->right, codes, prefix);
			prefix.pop_back();
		}
	}

	// Zapisuje drzewo do strumienia danych.
	void huffman_comp::saveTree(Node * root, std::ostream & output, Byte & accumulator, unsigned int& bitIndex) {
		if (root == NULL) {
			return;
		}
		if (bitIndex == 8) {
			output.write(reinterpret_cast<char*>(&accumulator), sizeof(accumulator));
			accumulator = 0;
			bitIndex = 0;
		}
		if (root->isLeaf()) {
			accumulator = setBit(accumulator, bitIndex);
			++bitIndex;
			if (bitIndex == 8) {
				output.write(reinterpret_cast<char*>(&accumulator), sizeof(accumulator));
				accumulator = 0;
				bitIndex = 0;
			}
			for (unsigned int i = 0; i < 8; ++i) {
				if (getBit(root->byte, i)) {
					accumulator = setBit(accumulator, bitIndex);
				}
				++bitIndex;
				if (bitIndex == 8) {
					output.write(reinterpret_cast<char*>(&accumulator), sizeof(accumulator));
					accumulator = 0;
					bitIndex = 0;
				}
			}
		}
		else {
			++bitIndex;
			if (bitIndex == 8) {
				output.write(reinterpret_cast<char*>(&accumulator), sizeof(accumulator));
				accumulator = 0;
				bitIndex = 0;
			}
			saveTree(root->left, output, accumulator, bitIndex);
			saveTree(root->right, output, accumulator, bitIndex);
		}
	}

	// Wczytuje drzewo ze strumienia danych.
	bool huffman_comp::loadTree(std::istream & input, Byte & accumulator, unsigned int& bitIndex, Node * &root) {
		if (bitIndex == 8) {
			if (!input.read(reinterpret_cast<char*>(&accumulator), sizeof(accumulator))) {
				return false;
			}
			bitIndex = 0;
		}
		root = new Node(0);
		bool bit = getBit(accumulator, bitIndex);
		++bitIndex;
		if (bit) {
			for (unsigned int i = 0; i < 8; ++i) {
				if (bitIndex == 8) {
					if (!input.read(reinterpret_cast<char*>(&accumulator), sizeof(accumulator))) {
						delete root;
						root = NULL;
						return false;
					}
					bitIndex = 0;
				}
				if (getBit(accumulator, bitIndex)) {
					root->byte = setBit(root->byte, i);
				}
				++bitIndex;
			}
		}
		else {
			if (!loadTree(input, accumulator, bitIndex, root->left)) {
				delete root;
				root = NULL;
				return false;
			}
			if (!loadTree(input, accumulator, bitIndex, root->right)) {
				Node::removeTree(root);
				root = NULL;
				return false;
			}
		}
		return true;
	}

	void huffman_comp::compress(std::istream& input, std::ostream& output) {
		Byte buffer;
		std::istream::pos_type start = input.tellg();

		std::vector<Node*> nodes(256); //tu beda siedziały jeszcze nie przypisane node'y
		for (unsigned int i = 0; i < 256; ++i) {
			nodes[i] = new Node(i);
		}

		// Przechodzimy po strumieniu wejściowym, żeby policzyć liczbę wystąpienia każdego bajtu.
		while (input.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
			++(nodes[buffer]->count);
		}

		// Tworzymy drzewo z wektora nodes
		std::sort(nodes.begin(), nodes.end(), Node::greaterThan);
		while (nodes.size() > 1) {
			Node* a, * b, * c;
			a = nodes.back();
			nodes.pop_back();
			b = nodes.back();
			nodes.pop_back();
			c = new Node(0, a, b, a->count + b->count);
			nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), c, Node::greaterThan), c);
		}
		Node * root = nodes.back();
		nodes.clear();

		Byte accumulator = 0; // Akumulator bitów.
		unsigned int bitIndex = 0; // Licznik bitów.

		saveTree(root, output, accumulator, bitIndex); // Zapisujemy drzewo do pliku

		// Dla usprawnienia dalszych operacji zapisujemy kody w tablicy.
		std::vector<bool> codes[256];
		std::vector<bool> a;
		mapTree(root, codes, a);

		Node::removeTree(root); // Drzewo już nie jest potrzebne.

		// Wracamy na początek strumienia danych.
		input.clear();
		input.seekg(start);

		while (input.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
			for (std::vector<bool>::const_iterator i = codes[buffer].begin(); i != codes[buffer].end(); ++i) {
				if (*i) {
					accumulator = setBit(accumulator, bitIndex);
				}
				++bitIndex;
				if (bitIndex == 8) {
					output.write(reinterpret_cast<char*>(&accumulator), sizeof(accumulator));
					bitIndex = 0;
					accumulator = 0;
				}
			}
		}
		if (bitIndex > 0) {
			output.write(reinterpret_cast<char*>(&accumulator), sizeof(accumulator));
			bitIndex = 0;
			accumulator = 0;
		}
	}

	bool huffman_comp::decompress(std::istream & input, std::ostream & output) {
		Node* root = NULL;

		Byte accumulator = 0;
		unsigned int bitIndex = 8;

		// Wczytujemy drzewo.
		if (!loadTree(input, accumulator, bitIndex, root)) {
			return false;
		}

		Node* current = root;
		while (true) {
			// Sprawdzamy czy nie ma błędu.
			if (current == NULL) {
				Node::removeTree(root); // Sprzątanie.
				return false;
			}

			if (current->isLeaf()) {
				// Zapisujemy jeden bajt do wyniku.
				output.write(reinterpret_cast<char*>(&(current->byte)), sizeof(current->byte));
				current = root;
			}

			// Odczytujemy kolejny bajt.
			if (bitIndex == 8) {
				if (!input.read(reinterpret_cast<char*>(&accumulator), sizeof(accumulator))) {
					break;
				}
				bitIndex = 0;
			}

			// Odczytujemy jeden bit.
			bool bit = getBit(accumulator, bitIndex);
			++bitIndex;
			if (bit == bLeft) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		Node::removeTree(root); // Sprzątanie.

		return true;
	}