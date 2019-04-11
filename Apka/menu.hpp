#pragma once

#define MENU_ID                         1
#define MENU_FILE_NEW                   10
#define MENU_FILE_OPEN_VIEW             11
#define MENU_FILE_OPEN_COMPVIEW_LZW     12
#define MENU_FILE_OPEN_COMPVIEW_HUFFMAN 13
#define MENU_FILE_EDIT                  14
#define MENU_FILE_COMPRESS_LZW          15
#define MENU_FILE_COMPRESS_HUFFMAN      16
#define MENU_JPEG                       17
#define MENU_INFO                       18
#define INFO_ID							"info.txt"
#define PROJECT_STATIC_DIR				L"C:\\Users\\Michal\\Desktop\\d¿ip d¿ip\\JiPP_projekt\\Apka"//TOFIX statyczna sciezka ale chociaz dziala
#define BUFFER_MAX_SIZE					3000000//maksymalna liczba znakow dla pliku z menuOpenView

namespace file {
	void menuNewFile(HWND hwnd);
	void menuOpenView(HWND hwnd);
	void menuOpenCompViewLZW(HWND hwnd);
	void menuOpenCompViewHuffman(HWND hwnd);
	void menuEdit(HWND hwnd);
	void menuCompressLZW(HWND hwnd);
	void menuCompressHuffman(HWND hwnd);
}

void testMenu(HWND window, LPCWSTR msg);
void menuJPEG(HWND hwnd);//stwórz okna input/output, okienka na liczby etc. uruchom algorytm i wyœwietl
void menuInfo(HWND hTextbox);