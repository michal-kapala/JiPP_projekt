#pragma once

#define MENU_ID                         1
#define MENU_FILE_SAVE                  10
#define MENU_FILE_OPEN	                11
#define MENU_FILE_COMPRESS_LZW          12
#define MENU_FILE_COMPRESS_HUFFMAN      13
#define MENU_JPEG                       14
#define MENU_INFO                       15
#define DIALOG_TITLE					16
#define DIALOG_COMPRESS					17
#define DIALOG_SHOW_EXAMPLE				18
#define DIALOG_TEXT_SOURCE				19
#define	DIALOG_TEXT_JPEG				20
#define DIALOG_TEXT_RESULT				21
#define POGGERS							30
#define INFO_ID							"info.txt"
#define PROJECT_STATIC_DIR				L"C:\\Users\\Michal\\Desktop\\d¿ip d¿ip\\JiPP_projekt\\Apka"//TOFIX statyczna sciezka ale chociaz dziala
#define BUFFER_MAX_SIZE					3000000//maksymalna liczba znakow dla pliku z menuOpenView

namespace file {
	void menuSaveFile(HWND hwnd);
	void menuOpen(HWND hwnd);
	void menuCompressLZW(HWND hwnd);
	void menuCompressHuffman(HWND hwnd);
}

void testMenu(HWND window, LPCWSTR msg);
void menuJPEG(HWND hwnd, LPCWSTR wndClassName);//stwórz okna input/output, okienka na liczby etc. uruchom algorytm i wyœwietl
void menuInfo(HWND hTextbox);
void clearTextbox(HWND hTextbox);