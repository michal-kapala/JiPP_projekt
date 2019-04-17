#include "stdafx.h"
#include "Huffman.h"
#include <algorithm>

void huffman::set_string_in(string in) {
	this->in = in;
}
string huffman::get_string_out() {
	return out;
}
key huffman::get_key() {
	return coding_key;
}
void huffman::set_key(key coding_key) {
	this->coding_key = coding_key;
}

void huffman::encode() {

	// Buliding alphabet
	string alphabet;
	vector<unsigned int> frequencies;

	for (int i = 0; i != in.size(); i++) {
		bool is_in_alphabet = 0;
		for (auto e : alphabet) {
			if (e == in[i]) is_in_alphabet = 1;
		}
		if (is_in_alphabet == 0) alphabet.push_back(in[i]);
	}
	frequencies.resize(alphabet.size());

	//Find frequencies
	for (int i = 0; i != alphabet.size(); i++) {
		unsigned int count = 0;
		for (auto e : in) {
			if (e == alphabet[i])count++;
		}
		frequencies[i] = count;
	}

	//Pass to huffman_encoder to get a key
	huffman_encoder encoder;
	encoder.set_alphabet(alphabet);
	encoder.set_frequencies(frequencies);
	coding_key = encoder.get_key();

	//proper encoding 
	//should work
	vector<bool> reinterpret;
	for (int i = 0; i != in.size(); i++) {
		char curr = in[i];
		vector<bool> curr_code = coding_key[curr];
		for (auto e : curr_code) {
			reinterpret.push_back(e);
		}
	}
	out = to_string(reinterpret);
}
void huffman::decode() {
	///TODO
}

std::size_t huffman::divide_rounding_up(std::size_t dividend, std::size_t divisor) {
	return (dividend + divisor - 1) / divisor;
}

std::string huffman::to_string(std::vector< bool > const & bitvector) {
	std::string ret(divide_rounding_up(bitvector.size(), 8), 0);
	auto out = ret.begin();
	int shift = 0;

	for (bool bit : bitvector) {
		*out |= bit << shift;

		if (++shift == 8) {
			++out;
			shift = 0;
		}
	}
	return ret;
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void huffman_encoder::set_frequencies(vector<unsigned int> frequencies) {
	this->frequencies = frequencies;
}

void huffman_encoder::set_alphabet(string alphabet) {
	this->alphabet = alphabet;
}

shared_ptr<tree> huffman_encoder::build_tree() {
	vector<shared_ptr<tree>> tree_list;
	string alphabet = this->alphabet;
	for (int i = alphabet.size(); i != 0; i--) {
		shared_ptr<tree> current = make_shared<tree>();
		unsigned int it = find_by_frequency();
		current->frequency = frequencies[it];
		current->letter = alphabet[it];
		tree_list.push_back(current);
		frequencies.erase(frequencies.begin() + it);
		alphabet.erase(alphabet.begin() + it);
	} //creating vector of trees of all letters with frequencies

	/*TODO:
	 -removing 2 least frequent letters from tree vec
	 -create a new tree with this 2 trees as leafs
	 -push it to vec
	 -repeatt till vec is empty
	 -you should end with 1 tree
	 -return it
	*/
	while (tree_list.size() != 1) {
		vector<shared_ptr<tree>>::iterator it1, it2;
		it1 = min_element(tree_list.begin(), tree_list.end(), [](shared_ptr<tree> uno, shared_ptr<tree> duo) {
			if (uno->frequency > duo->frequency) return uno;
			return duo;
		});
		shared_ptr<tree> it1_tree, it2_tree, push_tree = make_shared<tree>();
		it1_tree = *it1;
		tree_list.erase(it1);
		it2 = min_element(tree_list.begin(), tree_list.end(), [](shared_ptr<tree> uno, shared_ptr<tree> duo) {
			if (uno->frequency > duo->frequency) return uno;
			return duo;
		});
		it2_tree = *it2;
		push_tree->left = it1_tree;
		push_tree->right = it2_tree;
		tree_list.push_back(push_tree);
	}
	return tree_list[0];

	///////////////////////////////
	//OLD CODE:
	// works but it is not really huffman tree
	// new code should be more optimised
	//remove it later

	/*shared_ptr<tree> root = make_shared<tree>();
	shared_ptr<tree> current = root;

	for (int i = 1; i != alphabet.size(); i++) {
		current->right = make_shared<tree>(find_by_frequency_remove());
		current->left = make_shared<tree>();
		current = current->left;
	}

	current->right = make_shared<tree>(find_by_frequency_remove());
	current->left = make_shared<tree>(find_by_frequency_remove());
	return root;*/
}

unsigned int huffman_encoder::find_by_frequency() {
	unsigned int max = frequencies[0];
	unsigned int max_i = 0;
	for (int i = 0; i != frequencies.size(); i++) {
		if (frequencies[i] > max) {
			max = frequencies[i];
			max_i = i;
		}
	}
	return max_i;
}

key huffman_encoder::get_key() {
	tree_travel(build_tree());
	key return_key;
	/*return_key.alphabet = alphabet;
	return_key.codes = codes;*/
	for (int i = 0; i != alphabet.size(); i++) {
		return_key[alphabet[i]] = codes[i];
	}
	return return_key;
}

void huffman_encoder::tree_travel(shared_ptr<tree> point) {
	
}