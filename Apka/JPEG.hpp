#pragma once
#include <vector>
#include <windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <random>

class position {
public:
	int x, y;
	position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	void set_pos(int x, int y)
	{
		this->x += x;
		this->y += y;
	}
};

const int quant_table_50[8][8] = { {16, 11, 10, 16, 24, 40, 51, 51},
								{12, 12, 14, 19, 26, 58, 60, 55},
								{14, 13, 16, 24, 40, 57, 69, 56},
								{14, 17, 22, 29, 51, 87, 80, 62},
								{18, 22, 37, 56, 68, 109, 103, 77},
								{24, 35, 55, 64, 81, 104, 113, 92},
								{49, 64, 78, 87, 103,121, 120, 101},
								{72, 92, 95, 98, 112,100, 103, 99} };

double round_with_precision(const double &number);//do calkowite.x
std::vector<std::vector<double>> DCT(const std::vector<std::vector<int>> &input);
std::vector<double> zigzag(const std::vector<std::vector<double>>&matrix);
void create_quan_table(unsigned int quality, std::vector<std::vector<int>>&quantization_table);
template <typename T>
void save_table(const std::vector<std::vector<T>> &table, const std::string &desc, std::ofstream &file);
template <typename T>
void save_vector(const std::vector<T> &table, const std::string &desc, std::ofstream &file);
void rysuj_8x8(HWND hwnd);