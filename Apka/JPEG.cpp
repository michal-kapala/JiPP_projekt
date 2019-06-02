#include "stdafx.h"
#include "JPEG.hpp"
#include "my_huff.hpp"

template <typename T>
void save_table(const std::vector<std::vector<T>> &table, const std::string &desc, std::ofstream &file){
	file << desc;
	for (int x = 0; x < table.size(); x++) {
		for (int y = 0; y < table.size(); y++) {
			file << table[y][x] << " ";
		}
		file << std::endl;
	}
	file << std::endl;
}

template <typename T>
void save_vector(const std::vector<T> &table, const std::string &desc, std::ofstream &file){
	file << desc;
	for (auto e : table) {
		file << e << " ";
	}
	file << std::endl << std::endl;
}

void create_quan_table(unsigned int quality, std::vector<std::vector<int>> &quantization_table){
	double ratio;
	if (quality > 50)
		ratio = 2 - double(quality / 50);
	else
		ratio = double(50 / quality);
	for (int i = 0; i < 8; i++){
		std::vector<int>tmp;
		for (int j = 0; j<8; j++){
			tmp.push_back(round(double(quant_table_50[j][i] * ratio)));
		}
		quantization_table.push_back(tmp);
	}
}

double round_with_precision(const double &number){//do calkowite.x
	double out = double(number * 10);
	out = round(out);
	return double(out / 10);
}

int randomInt(int min, int max){
	static std::default_random_engine e{};
	std::uniform_int_distribution<int> d(min, max);
	return d(e);
}

std::vector<std::vector<double>> DCT(const std::vector<std::vector<int>> &input) {
	std::vector<std::vector<double>>output;
	std::vector<double>line;
	const double pi = 3.14159;
	double x_factor, y_factor, sum = 0, tmp_coef;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (!x)
				x_factor = 1 / sqrt(8);
			else
				x_factor = sqrt(2) / sqrt(8);
			if (!y)
				y_factor = 1 / sqrt(8);
			else
				y_factor = sqrt(2) / sqrt(8);
			sum = 0;
			for (int xx = 0; xx < 8; xx++) {
				for (int yy = 0; yy < 8; yy++) {
					tmp_coef = double(input[xx][yy] * cos((2 * xx + 1) * x * pi / 16) * cos((2 * yy + 1) * y * pi / 16));
					sum += tmp_coef;
				}
			}
			line.push_back(round_with_precision(x_factor * y_factor * sum));
		}
		output.push_back(line);
		line.clear();
	}
	return output;
}

std::vector<double> zigzag(const std::vector<std::vector<double>>&matrix){
	std::vector<double>out;

	out.push_back(matrix[0][0]);//lewy gorny naroznik
	position pos(1, 0);
	bool up_slant = false;

	while (pos.x <= 7 && pos.y <= 7){
		out.push_back(matrix[pos.x][pos.y]);
		//cout << pos.x << " " << pos.y << endl;
		if (pos.x == 7 && pos.y == 7)
			break;
		if (!pos.x && pos.y == 7){//lewy dolny naroznik
			pos.set_pos(1, 0);//w prawo
			up_slant = true;
			continue;
		}

		else if (pos.x == 7 && !pos.y){//prawy gorny naroznik
			pos.set_pos(-1, 1);//w dolny skos
			up_slant = false;
			continue;
		}

		else if (!pos.x){//lewa krawedz
			if (pos.y % 2)
				pos.set_pos(0, 1);//w dol
			else
				pos.set_pos(1, -1);//w gorny skos

			up_slant = true;
		}
		else if (pos.x == 7){//prawa krawedz
			if (pos.y % 2)
				pos.set_pos(0, 1);//w dol
			else
				pos.set_pos(-1, 1);//w dolny skos

			up_slant = false;
		}
		else if (!pos.y){//gorna krawedz
			if (pos.x % 2)
				pos.set_pos(-1, 1);//w dolny skos
			else
				pos.set_pos(1, 0);//w prawo

			up_slant = false;
		}
		else if (pos.y == 7){//dolna krawedz
			if (pos.x % 2)
				pos.set_pos(1, -1);//w gorny skos
			else
				pos.set_pos(1, 0);//w prawo

			up_slant = true;
		}
		else{//srodek tablicy - poruszamy sie po skosie
			if (up_slant)//w gore
				pos.set_pos(1, -1);
			else//albo w dol
				pos.set_pos(-1, 1);
		}
	}
	return out;
}

void rysuj_8x8(HWND hwnd){//8x8
	HDC hdcOkno = GetDC(hwnd);
	HBRUSH PedzelZiel, Pudelko;
	HPEN OlowekCzerw, Piornik;
	std::ofstream log("jpeg_log.txt");
	int x, y;
	std::vector<std::vector<int>> matrix;
	int luminosity, index_x = 0, index_y = 0;
	std::vector<std::vector<int>> exemplary_image;//obrocony o 90 stopni
	std::vector<int>v1{ 62, 62, 61, 63, 67, 82, 96, 109 };
	std::vector<int>v2{ 55, 57, 60, 61, 67, 95, 111, 121 };
	std::vector<int>v3{ 55, 54, 52, 60, 70, 101, 115, 127 };
	std::vector<int>v4{ 54, 52, 49, 60, 74, 106, 119, 133 };
	std::vector<int>v5{ 49, 48, 48, 63, 79, 114, 128, 139 };
	std::vector<int>v6{ 48, 47, 47, 65, 85, 115, 128, 141 };
	std::vector<int>v7{ 47, 48, 49, 68, 91, 112, 130, 140 };
	std::vector<int>v8{ 55, 53, 54, 65, 92, 117, 127, 133 };
	exemplary_image.push_back(v1);
	exemplary_image.push_back(v2);
	exemplary_image.push_back(v3);
	exemplary_image.push_back(v4);
	exemplary_image.push_back(v5);
	exemplary_image.push_back(v6);
	exemplary_image.push_back(v7);
	exemplary_image.push_back(v8);

	srand(time(0));
	for (x = 160; x < 480; x += 40) {//kolumny
		std::vector<int> tmp;
		for (y = 480; y < 800; y += 40){//wiersze
			luminosity = randomInt(78, 178);//zmniejszenie kontrastow
			PedzelZiel = CreateSolidBrush(RGB(luminosity, luminosity, luminosity));
			OlowekCzerw = CreatePen(PS_DOT, 1, 0x0000FF);//psychodela
			Pudelko = (HBRUSH)SelectObject(hdcOkno, PedzelZiel);
			Piornik = (HPEN)SelectObject(hdcOkno, OlowekCzerw);
			Rectangle(hdcOkno, x, y, 40 + x, 40 + y);
			SelectObject(hdcOkno, Pudelko);
			SelectObject(hdcOkno, Piornik);
			tmp.push_back(luminosity);

			DeleteObject(OlowekCzerw);
			DeleteObject(PedzelZiel);
		}
		matrix.push_back(tmp);
	}

	switch (matrix.size())
	{
	case 7:
		PedzelZiel = CreateSolidBrush(RGB(200, 20, 0));//czerwony
		OlowekCzerw = CreatePen(PS_DOT, 1, 0x0000FF);
		Pudelko = (HBRUSH)SelectObject(hdcOkno, PedzelZiel);
		Piornik = (HPEN)SelectObject(hdcOkno, OlowekCzerw);
		Rectangle(hdcOkno, 0, 0, 40, 40);
		SelectObject(hdcOkno, Pudelko);
		SelectObject(hdcOkno, Piornik);
		break;
	case 8:
		PedzelZiel = CreateSolidBrush(RGB(20, 200, 0));//zielony
		OlowekCzerw = CreatePen(PS_DOT, 1, 0x0000FF);
		Pudelko = (HBRUSH)SelectObject(hdcOkno, PedzelZiel);
		Piornik = (HPEN)SelectObject(hdcOkno, OlowekCzerw);
		Rectangle(hdcOkno, 0, 0, 40, 40);
		SelectObject(hdcOkno, Pudelko);
		SelectObject(hdcOkno, Piornik);
		break;
	case 9:
		PedzelZiel = CreateSolidBrush(RGB(0, 20, 200));//niebieski
		OlowekCzerw = CreatePen(PS_DOT, 1, 0x0000FF);
		Pudelko = (HBRUSH)SelectObject(hdcOkno, PedzelZiel);
		Piornik = (HPEN)SelectObject(hdcOkno, OlowekCzerw);
		Rectangle(hdcOkno, 0, 0, 40, 40);
		SelectObject(hdcOkno, Pudelko);
		SelectObject(hdcOkno, Piornik);
		break;
	default:
		PedzelZiel = CreateSolidBrush(RGB(200, 20, 200));//fioletowy
		OlowekCzerw = CreatePen(PS_DOT, 1, 0x0000FF);
		Pudelko = (HBRUSH)SelectObject(hdcOkno, PedzelZiel);
		Piornik = (HPEN)SelectObject(hdcOkno, OlowekCzerw);
		Rectangle(hdcOkno, 0, 0, 40, 40);
		SelectObject(hdcOkno, Pudelko);
		SelectObject(hdcOkno, Piornik);
		break;
	}
	//przykladowy blok
	index_x = index_y = 0;
	for (x = 160; x < 480; x += 40) {//kolumny
		for (y = 80; y < 400; y += 40){//wiersze
			PedzelZiel = CreateSolidBrush(RGB(exemplary_image[index_x][index_y], exemplary_image[index_x][index_y], exemplary_image[index_x][index_y]));
			OlowekCzerw = CreatePen(PS_DOT, 1, 0x0000FF);//psychodela
			Pudelko = (HBRUSH)SelectObject(hdcOkno, PedzelZiel);
			Piornik = (HPEN)SelectObject(hdcOkno, OlowekCzerw);
			Rectangle(hdcOkno, x, y, 40 + x, 40 + y);
			SelectObject(hdcOkno, Pudelko);
			SelectObject(hdcOkno, Piornik);

			DeleteObject(OlowekCzerw);
			DeleteObject(PedzelZiel);
			++index_y;
		}
		++index_x;
		index_y = 0;
	}
	save_table(matrix, "Tablica jasnoœci:\n", log);
	save_table(exemplary_image, "Przyk³adowa tablica jasnoœci:\n", log);
	//zmiana zakresu jasnosci przed DCT z <0, 256> na <-128, 128>
	for (index_x = 0; index_x < 8; index_x++) {
		for (index_y = 0; index_y < 8; index_y++){
			matrix[index_x][index_y] -= 128;
			exemplary_image[index_x][index_y] -= 128;
		}
			
	}
	save_table(matrix, "Przesuniêta tablica jasnoœci:\n", log);
	save_table(exemplary_image, "Przyk³adowa przesuniêta tablica jasnoœci:\n", log);

	std::vector<std::vector<double>>dct_table = DCT(matrix);//tablica wspolczynnikow
	save_table(dct_table, "Wspó³czynniki DCT:\n", log);

	std::vector<std::vector<double>>dct_table_ex = DCT(exemplary_image);
	save_table(dct_table_ex, "Przyk³adowe wspó³czynniki DCT:\n", log);

	std::vector<std::vector<int>>quantization_table;//tworzy tablice kwantyzacji o podanej jakosci
	create_quan_table(70, quantization_table);
	save_table(quantization_table, "Tablica kwantyzacji:\n", log);

	//kwantyzacja
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			dct_table[x][y] /= double(quantization_table[x][y]);
			dct_table[x][y] = round(dct_table[x][y]);

			dct_table_ex[y][x] /= double(quantization_table[x][y]);
			dct_table_ex[y][x] = round(dct_table_ex[y][x]);

			if (dct_table[x][y] == -0)
				dct_table[x][y] = 0;

			if (dct_table_ex[y][x] == -0)
				dct_table_ex[y][x] = 0;
		}
	}
	save_table(dct_table, "Skwantowana tablica wspó³czynników DCT:\n", log);
	save_table(dct_table_ex, "Skwantowana tablica przyk³adowych wspó³czynników DCT:\n", log);

	std::vector<double> quant_output = zigzag(dct_table);
	save_vector(quant_output, "Wektor do kompresji:\n", log);
	quant_output.clear();
	quant_output = zigzag(dct_table_ex);
	save_vector(quant_output, "Przykladowy wektor do kompresji:\n", log);
	std::ofstream tmp_file("vector_source.txt");
	save_vector(quant_output, "", tmp_file);
	tmp_file.close();

	log << "Wynik kompresji:\n";//ogólny output
	std::ifstream source("test.txt");//inty ujemne/dodatnie po spacji
	std::ofstream comp_output("comp_output.txt");//plik na krzaczory
	std::ofstream decomp_output("decomp_output.txt");//plik na zdekompresowane inty
	

	huffman_comp vector;
	
	vector.compress(source, comp_output);
	vector.compress(source, log);
	comp_output.close();
	std::ifstream comp_input("comp_output.txt");

	log << "\nWynik dekompresji:\n";
	vector.decompress(comp_input, decomp_output);
	vector.decompress(comp_input, log);
	
	source.close();
	decomp_output.close();

	ReleaseDC(hwnd, hdcOkno);
	log.close();
}