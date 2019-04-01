#include "stdafx.h"
#include <windows.h>
#include "menu.hpp"
//TODO, typy i argumenty do pozmieniania
void testMenu(HWND window, LPCWSTR msg) {
	MessageBox(window, msg, L"Menu test", MB_ICONINFORMATION);
}
	
void file::menuNewFile(HWND hwnd) {
	testMenu(hwnd, L"Zrób¿e nowy plik");
}

void file::menuOpenView(HWND hwnd)
{
	testMenu(hwnd, L"Otwórz¿e podgl¹d pliku Ÿród³owego");
}

void file::menuOpenCompViewLZW(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj¿e LZW i wyœwietl ³askawie");
}

void file::menuOpenCompViewHuffman(HWND hwnd)
{
	testMenu(hwnd, L"Skompresuj¿e Huffmankiem i wyœwietl ³askawie");
}

void file::menuEdit(HWND hwnd) {
	testMenu(hwnd, L"Masz poczêstuj siê edycj¹");
}

void file::menuCompressLZW(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj¿e LZW i eksportuj Bóg zap³aæ");
}

void file::menuCompressHuffman(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj¿e Huffmankiem i eksportuj tak jak Pan Jezus powiedzia³");
}

void menuJPEG(HWND hwnd) {
	testMenu(hwnd, L"HAHA NIE DLA PSA JPEG");
}

void menuHelp(HWND hwnd) {
	testMenu(hwnd, L"Nie no ¿artowa³em, masz, poczêstuj siê pomoc¹");
}