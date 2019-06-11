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

void create_quan_table(double quality, std::vector<std::vector<int>> &quantization_table){
	double ratio;
	if (quality > 50)
		ratio = double(2 - quality / 50);
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

std::vector<std::vector<int>> IDCT(const std::vector<std::vector<double>> &input) {
	std::vector<std::vector<int>>output;
	std::vector<int>line;
	const double pi = 3.14159;
	double sum=0, x_factor, y_factor, component2;

	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 8; y++) {
			sum = 0;
			for (int xx = 0; xx < 8; xx++) {
				for (int yy = 0; yy < 8; yy++) {
					component2 = input[xx][yy] * cos((2 * x + 1) * xx * pi / 16) * cos((2 * y + 1) * yy * pi / 16) * ((!xx) ? 1 / sqrt(2) : 1.) * ((!yy) ? 1 / sqrt(2) : 1.);
					sum += component2;
				}
			}
			line.push_back(sum/4);
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

std::vector<std::vector<double>> zigzag_matrix(const std::vector<double>&vec) {
	std::vector<std::vector<double>>out;
	std::vector<double>init{ 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 8; i++)
		out.push_back(init);
	int index = 0;
	out[0][0] = vec[index];//lewy gorny naroznik
	index++;
	position pos(1, 0);
	bool up_slant = false;

	while (pos.x <= 7 && pos.y <= 7) {
		out[pos.y][pos.x] = vec[index];
		if (pos.x == 7 && pos.y == 7)
			break;
		index++;
		if (!pos.x && pos.y == 7) {//lewy dolny naroznik
			pos.set_pos(1, 0);//w prawo
			up_slant = true;
			continue;
		}

		else if (pos.x == 7 && !pos.y) {//prawy gorny naroznik
			pos.set_pos(-1, 1);//w dolny skos
			up_slant = false;
			continue;
		}

		else if (!pos.x) {//lewa krawedz
			if (pos.y % 2)
				pos.set_pos(0, 1);//w dol
			else
				pos.set_pos(1, -1);//w gorny skos

			up_slant = true;
		}
		else if (pos.x == 7) {//prawa krawedz
			if (pos.y % 2)
				pos.set_pos(0, 1);//w dol
			else
				pos.set_pos(-1, 1);//w dolny skos

			up_slant = false;
		}
		else if (!pos.y) {//gorna krawedz
			if (pos.x % 2)
				pos.set_pos(-1, 1);//w dolny skos
			else
				pos.set_pos(1, 0);//w prawo

			up_slant = false;
		}
		else if (pos.y == 7) {//dolna krawedz
			if (pos.x % 2)
				pos.set_pos(1, -1);//w gorny skos
			else
				pos.set_pos(1, 0);//w prawo

			up_slant = true;
		}
		else {//srodek tablicy - poruszamy sie po skosie
			if (up_slant)//w gore
				pos.set_pos(1, -1);
			else//albo w dol
				pos.set_pos(-1, 1);
		}
	}
	return out;
}

void rysuj_przyklad(HWND hwnd, unsigned int quality){//8x8
	HDC hdcOkno = GetDC(hwnd);
	HBRUSH brush, brush_box;
	HPEN pen, pen_box;
	std::ofstream log("jpeg_log_ex.txt");
	int x, y;

	int  index_x = 0, index_y = 0;
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

	//przykladowy blok
	index_x = index_y = 0;
	for (x = 160; x < 480; x += 40) {//kolumny
		for (y = 40; y < 360; y += 40){//wiersze
			brush = CreateSolidBrush(RGB(exemplary_image[index_x][index_y], exemplary_image[index_x][index_y], exemplary_image[index_x][index_y]));
			pen = CreatePen(PS_DOT, 1, 0x0000FF);//psychodela
			brush_box = (HBRUSH)SelectObject(hdcOkno, brush);
			pen_box = (HPEN)SelectObject(hdcOkno, pen);
			Rectangle(hdcOkno, x, y, 40 + x, 40 + y);
			SelectObject(hdcOkno, brush_box);
			SelectObject(hdcOkno, pen_box);

			DeleteObject(pen);
			DeleteObject(brush);
			++index_y;
		}
		++index_x;
		index_y = 0;
	}
	
	save_table(exemplary_image, "Przyk³adowa tablica jasnoœci:\n", log);
	//zmiana zakresu jasnosci przed DCT z <0, 256> na <-128, 128>
	for (index_x = 0; index_x < 8; index_x++) {
		for (index_y = 0; index_y < 8; index_y++)
			exemplary_image[index_x][index_y] -= 128;
			
	}
	
	save_table(exemplary_image, "Przyk³adowa przesuniêta tablica jasnoœci:\n", log);
	std::vector<std::vector<double>>dct_table_ex = DCT(exemplary_image);
	save_table(dct_table_ex, "Przyk³adowe wspó³czynniki DCT:\n", log);

	std::vector<std::vector<int>>quantization_table;//tworzy tablice kwantyzacji o podanej jakosci
	create_quan_table(quality, quantization_table);
	save_table(quantization_table, "Tablica kwantyzacji:\n", log);

	//kwantyzacja
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			dct_table_ex[y][x] /= double(quantization_table[x][y]);
			dct_table_ex[y][x] = round(dct_table_ex[y][x]);

			if (dct_table_ex[y][x] == -0)
				dct_table_ex[y][x] = 0;
		}
	}
	save_table(dct_table_ex, "Skwantowana tablica przyk³adowych wspó³czynników DCT:\n", log);

	std::vector<double> quant_output = zigzag(dct_table_ex);
	save_vector(quant_output, "Przyk³adowy wektor do kompresji:\n", log);
	std::ofstream tmp_file("vector_source.txt");
	save_vector(quant_output, "", tmp_file);
	tmp_file.close();

	log << "Wynik kompresji przyk³adu:\n";//ogólny output
	std::ifstream source("vector_source.txt");//inty ujemne/dodatnie po spacji
	std::ofstream comp_output("comp_output.txt", std::ios::binary);//plik na krzaczory
	std::ofstream decomp_output("decomp_output.txt");//plik na zdekompresowane inty
	
	huffman_comp huff;
	
	huff.compress(source, comp_output);//kompresja
	huff.compress(source, log);
	comp_output.close();
	std::ifstream comp_input("comp_output.txt", std::ios::binary);

	log << "\nWynik dekompresji przyk³adu:\n";
	huff.decompress(comp_input, decomp_output);//dekompresja

	source.close();
	decomp_output.close();
	comp_input.close();

	source.open("decomp_output.txt");//zapis
	int number;
	std::vector<double>quant_vector_input;//wektor do przeksztalcenia w macierz
	while (source >> number) {
		log << number << " ";
		quant_vector_input.push_back(double(number));
	}
	log << std::endl;

	std::vector<std::vector<double>> quant_input;
	quant_input = zigzag_matrix(quant_vector_input);//wektor przetworzony w macierz
	save_table(quant_input, "\nMacierz przyk³adowych zdekompresowanych wspó³czynników DCT:\n", log);

	for (int i = 0; i < 8;i++) {//dekwantyzacja
		for (int j = 0; j < 8; j++)
			quant_input[i][j] *= quantization_table[i][j];
	}
	save_table(quant_input, "\nMacierz przyk³adowych zdekwantyzowanych wspó³czynników DCT:\n", log);

	std::vector<std::vector<int>>decompressed_output = IDCT(quant_input);//odwrotna DCT
	log << "\nZdekompresowany przesuniêty przyk³ad:\n";//zapis
	for (auto v : decompressed_output) {
		for (auto e : v)
			log << e << " ";
		log << std::endl;
	}
	log<<"\nZdekompresowany przyk³ad:\n";
	for (int i = 0; i < 8;i++) {
		for (int j = 0; j < 8; j++) {
			decompressed_output[i][j] += 128;
			log << decompressed_output[i][j] << " ";
		}
		log << std::endl;
	}

	index_x = index_y = 0;//wyswietlanie przykladu
	for (x = 720; x < 1040; x += 40) {//kolumny
		for (y = 40; y < 360; y += 40) {//wiersze
			brush = CreateSolidBrush(RGB(decompressed_output[index_y][index_x], decompressed_output[index_y][index_x], decompressed_output[index_y][index_x]));
			pen = CreatePen(PS_DOT, 1, 0x0000FF);
			brush_box = (HBRUSH)SelectObject(hdcOkno, brush);
			pen_box = (HPEN)SelectObject(hdcOkno, pen);
			Rectangle(hdcOkno, x, y, 40 + x, 40 + y);
			SelectObject(hdcOkno, brush_box);
			SelectObject(hdcOkno, pen_box);

			DeleteObject(pen);
			DeleteObject(brush);
			++index_y;
		}
		++index_x;
		index_y = 0;
	}

	ReleaseDC(hwnd, hdcOkno);
	log.close();
}

void rysuj_8x8(HWND hwnd, unsigned int quality)
{
	HDC hdcOkno = GetDC(hwnd);
	HBRUSH brush, brush_box;
	HPEN pen, pen_box;
	std::ofstream log("jpeg_log.txt");
	int x, y;
	std::vector<std::vector<int>> matrix;
	int luminosity, index_x = 0, index_y = 0;
	srand(time(0));
	for (x = 160; x < 480; x += 40) {//kolumny
		std::vector<int> tmp;
		for (y = 480; y < 800; y += 40) {//wiersze
			luminosity = randomInt(78, 178);//zmniejszenie kontrastow
			brush = CreateSolidBrush(RGB(luminosity, luminosity, luminosity));
			pen = CreatePen(PS_DOT, 1, 0x0000FF);//psychodela
			brush_box = (HBRUSH)SelectObject(hdcOkno, brush);
			pen_box = (HPEN)SelectObject(hdcOkno, pen);
			Rectangle(hdcOkno, x, y, 40 + x, 40 + y);
			SelectObject(hdcOkno, brush_box);
			SelectObject(hdcOkno, pen_box);
			tmp.push_back(luminosity);

			DeleteObject(pen);
			DeleteObject(brush);
		}
		matrix.push_back(tmp);
	}
	save_table(matrix, "Tablica jasnoœci:\n", log);
	//zmiana zakresu jasnosci przed DCT z <0, 256> na <-128, 128>
	for (index_x = 0; index_x < 8; index_x++) {
		for (index_y = 0; index_y < 8; index_y++)
			matrix[index_x][index_y] -= 128;

	}
	save_table(matrix, "Przesuniêta tablica jasnoœci:\n", log);
	std::vector<std::vector<double>>dct_table = DCT(matrix);//tablica wspolczynnikow
	save_table(dct_table, "Wspó³czynniki DCT:\n", log);
	
	std::vector<std::vector<int>>quantization_table;//tworzy tablice kwantyzacji o podanej jakosci
	create_quan_table(quality, quantization_table);
	save_table(quantization_table, "Tablica kwantyzacji:\n", log);

	//kwantyzacja
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			dct_table[x][y] /= double(quantization_table[x][y]);
			dct_table[x][y] = round(dct_table[x][y]);

			if (dct_table[x][y] == -0)
				dct_table[x][y] = 0;
		}
	}
	save_table(dct_table, "Skwantowana tablica wspó³czynników DCT:\n", log);
	std::vector<double> quant_output = zigzag(dct_table);
	save_vector(quant_output, "Wektor do kompresji:\n", log);
	
	std::ofstream tmp_file("vector_source.txt");
	save_vector(quant_output, "", tmp_file);
	tmp_file.close();

	log << "Wynik kompresji:\n";//ogólny output
	std::ifstream source("vector_source.txt");//inty ujemne/dodatnie po spacji
	std::ofstream comp_output("comp_output.txt", std::ios::binary);//plik na krzaczory
	std::ofstream decomp_output("decomp_output.txt");//plik na zdekompresowane inty

	huffman_comp huff;

	huff.compress(source, comp_output);//kompresja
	huff.compress(source, log);
	comp_output.close();
	std::ifstream comp_input("comp_output.txt", std::ios::binary);

	log << "\nWynik dekompresji:\n";
	huff.decompress(comp_input, decomp_output);//dekompresja

	source.close();
	decomp_output.close();
	comp_input.close();

	source.open("decomp_output.txt");//zapis
	int number;
	std::vector<double>quant_vector_input;//wektor do przeksztalcenia w macierz
	while (source >> number) {
		log << number << " ";
		quant_vector_input.push_back(double(number));
	}
	log << std::endl;

	std::vector<std::vector<double>> quant_input;
	quant_input = zigzag_matrix(quant_vector_input);//wektor przetworzony w macierz
	save_table(quant_input, "\nMacierz zdekompresowanych wspó³czynników DCT:\n", log);
	for (int i = 0; i < 8; i++) {//dekwantyzacja
		for (int j = 0; j < 8; j++)
			quant_input[i][j] *= quantization_table[i][j];
	}
	save_table(quant_input, "\nMacierz przyk³adowych zdekwantyzowanych wspó³czynników DCT:\n", log);

	std::vector<std::vector<int>>decompressed_output = IDCT(quant_input);//odwrotna DCT
	log << "\nZdekompresowany przesuniêty przyk³ad:\n";//zapis
	for (auto v : decompressed_output) {
		for (auto e : v)
			log << e << " ";
		log << std::endl;
	}
	log << "\nZdekompresowany przyk³ad:\n";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			decompressed_output[i][j] += 128;
			log << decompressed_output[i][j] << " ";
		}
		log << std::endl;
	}

	index_x = index_y = 0;//wyswietlanie przykladu
	for (x = 720; x < 1040; x += 40) {//kolumny
		for (y = 480; y < 800; y += 40) {//wiersze
			brush = CreateSolidBrush(RGB(decompressed_output[index_y][index_x], decompressed_output[index_y][index_x], decompressed_output[index_y][index_x]));
			pen = CreatePen(PS_DOT, 1, 0x0000FF);
			brush_box = (HBRUSH)SelectObject(hdcOkno, brush);
			pen_box = (HPEN)SelectObject(hdcOkno, pen);
			Rectangle(hdcOkno, x, y, 40 + x, 40 + y);
			SelectObject(hdcOkno, brush_box);
			SelectObject(hdcOkno, pen_box);

			DeleteObject(pen);
			DeleteObject(brush);
			++index_y;
		}
		++index_x;
		index_y = 0;
	}

	ReleaseDC(hwnd, hdcOkno);
	log.close();
}