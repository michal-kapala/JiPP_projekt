#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <map>
#include <string>

class LZW_comp {
private:
	unsigned short int char_bits;
	std::map<std::string, unsigned short int> dictionary;
	std::map<unsigned short int, std::string> re_dictionary;

	void clear_dictionary() {
		dictionary.clear();
		char character = 0;
		for (unsigned int i = 0; i != 256; i++) {
			dictionary[std::string({ character })] = i;
			character++;
		}
	}
	bool dictionary_contains(std::string contains) { //checks if dic contains key
		std::map<std::string, unsigned short int>::iterator it = dictionary.find(contains);
		if (it == dictionary.end()) return false;
		return true;
	}
	void fill_re_dic() {
		re_dictionary.clear();
		for (auto e : dictionary) {
			re_dictionary[e.second] = e.first;
		}
	}
	bool dictionary_is_full() {
		if ((dictionary.size() + 1) == pow(2, char_bits)) return true;
		return false;
	}

public:
	LZW_comp(unsigned int bits_used = 10) {
		char_bits = bits_used;
		if (char_bits < 8) char_bits = 10;
		clear_dictionary();
	}

	std::vector<unsigned short int> compress(LPSTR input) {
		clear_dictionary();
		std::string current{ input[0] };
		std::string next;
		std::vector<unsigned short int> output;
		char* iter = input;
		char* end = input + strlen(input);


		for (iter; iter != end; iter++) {
			next = *iter;
			if (dictionary_contains(current+next)) {
				current = current + next;
				continue;
			}
			output.push_back(dictionary[current]);
			if (!dictionary_is_full()) dictionary[current + next] = dictionary.size();
			current = next;
		}
		return output;
	}

	std::string decompress(std::vector<unsigned short int> input_n) {
		std::string output_string;
		clear_dictionary();
		fill_re_dic();
		unsigned short int current = input_n[0] ;
		std::string next;
		output_string.push_back(*re_dictionary[current].c_str());
		next.push_back(*re_dictionary[current].c_str());

		for (int i = 1; i != input_n.size(); i++) {
			current = input_n[i];
			std::string temp = re_dictionary[current];
			std::string temp2;
			temp2.push_back(*next.c_str());
			temp2.push_back(temp[0]);
			re_dictionary[re_dictionary.size()] = temp2;
			next = temp;
			temp = re_dictionary[current];
			output_string.push_back(*temp.c_str());
		}
		return output_string;
	}

};