#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
using namespace std;
using code_h = vector<bool>;
using key = map<char, code_h>; //flat tree


struct tree {
	shared_ptr<tree> left;
	shared_ptr<tree> right;
	char letter;
	unsigned int frequency;
	tree() {}
	tree(char value) : letter(value) {}
	tree(char value1, unsigned int value2) : letter(value1), frequency(value2) {}
};

/*	Takes in frequencies, alphabet 
	Gives out key
*/
class huffman_encoder {
private:
	vector<unsigned int> frequencies;
	map<char,code_h> codes;
	string alphabet;
	shared_ptr<tree> build_tree();
	unsigned int find_by_frequency();
	void tree_travel(shared_ptr<tree> point);
public:
	void set_frequencies(vector<unsigned int> frequencies);
	void set_alphabet(string alphabet);
	key get_key(); 
};



/*
Huffman class:
	-Takes string to encode and gives key and encoded string

	-Takes string and key to decode and gives out decoded string
*/
class huffman {
private:
	std::size_t divide_rounding_up(std::size_t dividend, std::size_t divisor);
	std::string to_string(std::vector< bool > const & bitvector);
	string in;
	string out;
	key coding_key;
public:
	void set_string_in(string in);
	string get_string_out();
	key get_key();
	void set_key(key coding_key);

	void encode(); //Takes string, creates alphabet, frequencies, passes to huffman_encoder class to get a key, and encodes
	void decode(); //Takes string and key and decodes
};