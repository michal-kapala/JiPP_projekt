#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <map>
#include <fstream>
#include <iostream>
#include <Windows.h>
//
//template <typename T, typename K>
//bool map_contains(std::map<K, T> map, K contains) {
//	std::map<K, T>::iterator it = map.find(contains);
//	if (it == map.end()) return false;
//	return true;
//}

union bit_character {
	unsigned char character;
	bool bit[10];
};

union bit_int {
	bit_int(int char_bits) {
		bit = new bool[char_bits];
	}
	unsigned short int integer;
	bool* bit;
};

class LZW_comp {
private:
	std::map<std::string, unsigned int> dictionary; //dic
	std::map<unsigned int, std::string> re_dictionary;//reversed
	unsigned int char_bits; // this number defines how long in bits will be 1 char compressed (default 10)

	bool getBit(unsigned short int value, int position) // position in range 0-15
	{
		return (value >> position) & 0x1;
	}

	void setBit(unsigned char& value, unsigned int index, bool to_what) {
		if (to_what) value |= (1 << index);
		else value &= ~(1 << index);
	}

	void setBit(unsigned short int& value, unsigned int index, bool to_what) {
		if (to_what) value |= (1 << index);
		else value &= ~(1 << index);
	}

	void fill_re_dic() {
		re_dictionary.clear();
		for (auto e : dictionary) {
			re_dictionary[e.second] = e.first;
		}
	}

	void clear_dictionary() { // clears dic and fills it with ASCII table
		dictionary.clear();
		///NOTE: this is not necessary, but it is not a mistake\
		i coded it, before i realized that
		char character = 0;
		for (unsigned int i = 0; i != 256; i++) {
			dictionary[std::string({ character })] = i;
			character++;
		}
	}
	bool dictionary_contains(std::string contains) { //checks if dic contains key
		std::map<std::string, unsigned int>::iterator it = dictionary.find(contains);
		if (it == dictionary.end()) return false;
		return true;
	}
	bool re_dictionary_contains(unsigned int contains) { //checks if dic contains key
		std::map<unsigned int, std::string>::iterator it = re_dictionary.find(contains);
		if (it == re_dictionary.end()) return false;
		return true;
	}
	bool dictionary_contains_value(unsigned int value) {
		for (auto e : dictionary) {
			if (e.second == value) return true;
		}
		return false;
	}
	std::string dictionary_find_value(unsigned int value) {
		for (auto e : dictionary) {
			if (e.second == value) return e.first;
		}
		return std::string();
	}

public:
	LZW_comp(unsigned int char_bits = 10) { //default constructor
		///NOTE: code in current stage will chrash if char_bits is set to anything but 10
		if (char_bits < 8) char_bits = 10;
		clear_dictionary();
		this->char_bits = char_bits;
	}

	std::string compress(LPSTR input) { //LPSTR - long pointer string = char*
		clear_dictionary();
		std::vector<unsigned short int> output_n; //will contain numerical values of output string

		//////////////////////////////////////////////////////////////////////////////////
		//this loop converts input to numerical values and saves them in order in output_n
		//////////////////////////////////////////////////////////////////////////////////
		for (int i = 0; i != strlen(input); i++) { ///this code is a mess, but works
			std::string current_symbol; //will be checked if present in dictionary
			bool dictionary_full = 1; //0 means full
			unsigned int letters_left = strlen(input) - i - 1;
			//filling it with letters
			current_symbol.push_back(input[i]);
		FILL:
			letters_left = strlen(input) - i - 1;
			int offset = 1;

			if (letters_left) current_symbol.push_back(input[i + offset]);
			else { //end of input case
				if (dictionary_contains(current_symbol)) {
					output_n.push_back(dictionary[current_symbol]);
				}
				else {
					current_symbol.pop_back();
					output_n.push_back(dictionary[current_symbol]);
					std::string last_letter({ input[i + offset] });
					output_n.push_back(dictionary[last_letter]);
				}
				break;
			}

			if (dictionary_contains(current_symbol) && dictionary_full) { //dic already contains current_symbol
				i++;
				goto FILL;
			}
			else { //dic does not contain current_symbol
				if ((dictionary.size() + 1) < pow(2, char_bits)) { // if dic is not full 
					dictionary[current_symbol] = dictionary.size();
					current_symbol.pop_back();
					output_n.push_back(dictionary[current_symbol]);
				}
				else { //dic is full - do not add, use existing symbols
					current_symbol.pop_back();
					output_n.push_back(dictionary[current_symbol]);
					dictionary_full = 0;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////////////
		//this part converts output_n to binary values with length of chat_bits
		//////////////////////////////////////////////////////////////////////////////////

		//attempt 2
	//	std::string processed;
	//	bit_int current_save(char_bits);
	//	bit_character current_save_char;
	//	int i1 = 0;
	//	int i2 = 0;
	//	std::vector<unsigned short int>::iterator i_input = output_n.begin();
	//	current_save.integer = *i_input;

	///*start:
	//	current_save_char.bit[i1] = current_save.bit[i2];
	//	i1++;
	//	i2++;
	//	if (i1 == 8) goto char_over;
	//	if (i2 == char_bits) goto int_over;
	//	goto start;

	//char_over:
	//	processed.push_back(current_save_char.character);
	//	i1 = 0;
	//	if (i2 == char_bits) goto int_over;
	//	goto start;

	//int_over:
	//	i_input++;
	//	if (i_input == output_n.end()) goto end;
	//	current_save.integer = *i_input;
	//	i2 = 0;
	//	goto start;

	//end:
	//	processed.push_back(current_save_char.character);
	//	output = (LPSTR)processed.c_str();
	//	return void();
	//}*/

		//attempt 3
		/*std::string output_char(output_n.begin(), output_n.end());
		output = (LPSTR)output_char.c_str();
		return void();*/

		//attempt 4&5&6&7&9
		//6-partial success
		std::string processed;
		std::vector<unsigned short int>::iterator output_it = output_n.begin();
		int char_it = 0;
		unsigned short int integer;
		unsigned char character;

		while (output_it != output_n.end()) {
			int int_it = 0;
			integer = *output_it;
			while (int_it != char_bits) {
				if (char_it == 8) {
					processed.push_back(character);
					char_it = 0;
				}
				setBit(character, 8-char_it, getBit(integer, char_bits-int_it));
				char_it++;
				int_it++;
			}
			output_it++;
		}
		processed.push_back(character);
		return processed;
		//attempt 8
		/*while (output_it != output_n.end()) {
			unsigned short int multi_int[4];
			unsigned char multi[5];
			for (int i = 0; i != 4; i++) {
				if (output_it != output_n.end())multi_int[i] = *output_it;
				else multi_int[i] = 0;
				if (output_it != output_n.end()) output_it++;
			}
				setBit(multi[0], 7, getBit(multi_int[0], 9));
				setBit(multi[0], 6, getBit(multi_int[0], 8));
				setBit(multi[0], 5, getBit(multi_int[0], 7));
				setBit(multi[0], 4, getBit(multi_int[0], 6));
				setBit(multi[0], 3, getBit(multi_int[0], 5));
				setBit(multi[0], 2, getBit(multi_int[0], 4));
				setBit(multi[0], 1, getBit(multi_int[0], 3));
				setBit(multi[0], 0, getBit(multi_int[0], 2));
				setBit(multi[1], 7, getBit(multi_int[0], 1));
				setBit(multi[1], 6, getBit(multi_int[0], 0));
				setBit(multi[1], 5, getBit(multi_int[1], 9));
				setBit(multi[1], 4, getBit(multi_int[1], 8));
				setBit(multi[1], 3, getBit(multi_int[1], 7));
				setBit(multi[1], 2, getBit(multi_int[1], 6));
				setBit(multi[1], 1, getBit(multi_int[1], 5));
				setBit(multi[1], 0, getBit(multi_int[1], 4));
				setBit(multi[2], 7, getBit(multi_int[1], 3));
				setBit(multi[2], 6, getBit(multi_int[1], 2));
				setBit(multi[2], 5, getBit(multi_int[1], 1));
				setBit(multi[2], 4, getBit(multi_int[1], 0));
				setBit(multi[2], 3, getBit(multi_int[1], 9));
				setBit(multi[2], 2, getBit(multi_int[2], 8));
				setBit(multi[2], 1, getBit(multi_int[2], 7));
				setBit(multi[2], 0, getBit(multi_int[2], 6));
				setBit(multi[3], 7, getBit(multi_int[2], 5));
				setBit(multi[3], 6, getBit(multi_int[2], 4));
				setBit(multi[3], 5, getBit(multi_int[2], 3));
				setBit(multi[3], 4, getBit(multi_int[2], 2));
				setBit(multi[3], 3, getBit(multi_int[2], 1));
				setBit(multi[3], 2, getBit(multi_int[2], 0));
				setBit(multi[3], 1, getBit(multi_int[2], 9));
				setBit(multi[3], 0, getBit(multi_int[3], 8));
				setBit(multi[4], 7, getBit(multi_int[3], 7));
				setBit(multi[4], 6, getBit(multi_int[3], 6));
				setBit(multi[4], 5, getBit(multi_int[3], 5));
				setBit(multi[4], 4, getBit(multi_int[3], 4));
				setBit(multi[4], 3, getBit(multi_int[3], 3));
				setBit(multi[4], 2, getBit(multi_int[3], 2));
				setBit(multi[4], 1, getBit(multi_int[3], 1));
				setBit(multi[4], 0, getBit(multi_int[3], 0));
			for (int i = 0; i != 4; i++) {
				processed.push_back(multi[i]);
			}
		}

		return processed;
		*/
	}


	std::string decompress(std::string input) {
		//attempt 2
		/*std::string converter(input);
		std::vector<unsigned short int> input_n (converter.begin(),converter.end());
*/
//attempt 1
/*bit_character curr_char;
bit_int curr_int(char_bits);
std::vector<unsigned short int> input_n;
int it1=0;
int it2=0;
int it3=1;
curr_char.character = input[0];

start:
	curr_int.bit[it1] = curr_char.bit[it2];
	it1++;
	it2++;
	if (it2 == 8) goto char_over;
	if (it1 == char_bits) goto int_over;
	goto start;

char_over:
	it2 = 0;
	it3++;
	if (it3 == strlen(input)-1) goto end;
	curr_char.character = input[it3];
	if (it1 == char_bits) goto int_over;
	goto start;

int_over:
	it1 = 0;
	input_n.push_back(curr_int.integer);
	goto start;

end:
	input_n.push_back(curr_int.integer);
*/
//attempt 3&4

		std::string::iterator input_it = input.begin();
		std::vector<unsigned short int> input_n;
		unsigned short int integer = 0;
		unsigned char character;
		int int_it = 0;
		/*while (input_it != input.end()) {
			int char_it = 0;
			character = *input_it;
			while (char_it != 8) {
				if (int_it == char_bits) {
					input_n.push_back(integer);
					integer = 0;
					int_it = 0;
				}
				setBit(integer,char_bits-int_it, getBit(character,8-char_it));
				char_it++;
				int_it++;
			}
			input_it++;

		}*/

		while (input_it != input.end()) {
			unsigned short int multi_int[4];
			unsigned char multi[5];
			for (int i = 0; i != 5; i++) {
				if (input_it != input.end())multi[i] = *input_it;
				else multi[i] = 0;
				if (input_it != input.end()) input_it++;
			}
			setBit(multi_int[0], 9, getBit(multi[0], 7));
			setBit(multi_int[0], 8, getBit(multi[0], 6));
			setBit(multi_int[0], 7, getBit(multi[0], 5));
			setBit(multi_int[0], 6, getBit(multi[0], 4));
			setBit(multi_int[0], 5, getBit(multi[0], 3));
			setBit(multi_int[0], 4, getBit(multi[0], 2));
			setBit(multi_int[0], 3, getBit(multi[0], 1));
			setBit(multi_int[0], 2, getBit(multi[0], 0));
			setBit(multi_int[0], 1, getBit(multi[1], 7));
			setBit(multi_int[0], 0, getBit(multi[1], 6));
			setBit(multi_int[1], 9, getBit(multi[1], 5));
			setBit(multi_int[1], 8, getBit(multi[1], 4));
			setBit(multi_int[1], 7, getBit(multi[1], 3));
			setBit(multi_int[1], 6, getBit(multi[1], 2));
			setBit(multi_int[1], 5, getBit(multi[1], 1));
			setBit(multi_int[1], 4, getBit(multi[1], 0));
			setBit(multi_int[1], 3, getBit(multi[2], 7));
			setBit(multi_int[1], 2, getBit(multi[2], 6));
			setBit(multi_int[1], 1, getBit(multi[2], 5));
			setBit(multi_int[1], 0, getBit(multi[2], 4));
			setBit(multi_int[2], 9, getBit(multi[2], 3));
			setBit(multi_int[2], 8, getBit(multi[2], 2));
			setBit(multi_int[2], 7, getBit(multi[2], 1));
			setBit(multi_int[2], 6, getBit(multi[2], 0));
			setBit(multi_int[2], 5, getBit(multi[3], 7));
			setBit(multi_int[2], 4, getBit(multi[3], 6));
			setBit(multi_int[2], 3, getBit(multi[3], 5));
			setBit(multi_int[2], 2, getBit(multi[3], 4));
			setBit(multi_int[2], 1, getBit(multi[3], 3));
			setBit(multi_int[2], 0, getBit(multi[3], 2));
			setBit(multi_int[3], 9, getBit(multi[3], 1));
			setBit(multi_int[3], 8, getBit(multi[3], 0));
			setBit(multi_int[3], 7, getBit(multi[4], 7));
			setBit(multi_int[3], 6, getBit(multi[4], 6));
			setBit(multi_int[3], 5, getBit(multi[4], 5));
			setBit(multi_int[3], 4, getBit(multi[4], 4));
			setBit(multi_int[3], 3, getBit(multi[4], 3));
			setBit(multi_int[3], 2, getBit(multi[4], 2));
			setBit(multi_int[3], 1, getBit(multi[4], 1));
			setBit(multi_int[3], 0, getBit(multi[4], 0));
			for (int i = 0; i != 4; i++) {
				input_n.push_back(multi_int[i]);
			}
		}

		//at this point the string is in numerical form in input_n
		std::string output_string;
		clear_dictionary();
		fill_re_dic();

		for (unsigned int i = 0; i != input_n.size(); i++) {
			std::string push_char;
			output_string.push_back(*re_dictionary[input_n[i]].c_str());
			push_char.push_back(*re_dictionary[input_n[i]].c_str());
			if (i != input_n.size() - 1) push_char.push_back(re_dictionary[input_n[i + 1]][0]);
			re_dictionary[re_dictionary.size()] = push_char;
		}
		return output_string;

		//algo https://www.youtube.com/watch?v=j2HSd3HCpDs
	}

};