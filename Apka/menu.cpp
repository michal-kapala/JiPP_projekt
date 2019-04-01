#include "stdafx.h"
#include <windows.h>
#include "menu.hpp"
//TODO, typy i argumenty do pozmieniania
void testMenu(HWND window, LPCWSTR msg) {
	MessageBox(window, msg, L"Menu test", MB_ICONINFORMATION);
}
	
void file::menuNewFile(HWND hwnd) {
	testMenu(hwnd, L"Zr�b�e nowy plik");
}

void file::menuOpenView(HWND hwnd)
{
	testMenu(hwnd, L"Otw�rz�e podgl�d pliku �r�d�owego");
}

void file::menuOpenCompViewLZW(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj�e LZW i wy�wietl �askawie");
}

void file::menuOpenCompViewHuffman(HWND hwnd)
{
	testMenu(hwnd, L"Skompresuj�e Huffmankiem i wy�wietl �askawie");
}

void file::menuEdit(HWND hwnd) {
	testMenu(hwnd, L"Masz pocz�stuj si� edycj�");
}

void file::menuCompressLZW(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj�e LZW i eksportuj B�g zap�a�");
}

void file::menuCompressHuffman(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj�e Huffmankiem i eksportuj tak jak Pan Jezus powiedzia�");
}

void menuJPEG(HWND hwnd) {
	testMenu(hwnd, L"HAHA NIE DLA PSA JPEG");
}

void menuHelp(HWND hwnd) {
	testMenu(hwnd, L"Nie no �artowa�em, masz, pocz�stuj si� pomoc�");
}