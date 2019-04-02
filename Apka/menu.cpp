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

void menuInfo(HWND hTextbox) {//TODO EOF-read, gdzies jest wyciek pamieci bo sie wypierdala po wielu probach, callback dla textboxa mo�e pom�c
	HANDLE hInfo = CreateFile(INFO_ID, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dSize = GetFileSize(hInfo, NULL), dRead;
	LPSTR buffer = (LPSTR)GlobalAlloc(GPTR, dSize + 1);
	
	if (!ReadFile(hInfo, buffer, dSize, &dRead, NULL)) {
		MessageBox(NULL, L"File reading process failed", L"Error", MB_ICONEXCLAMATION);
		GlobalFree(buffer);
	}
	else {
		buffer += NULL;
		LPSTR unicoded = (LPSTR)GlobalAlloc(GPTR, dSize + 1);
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, (LPWSTR)unicoded, dSize + 1);//JEBAC
		GlobalFree(buffer);
		SetWindowText(hTextbox, (LPCWSTR)unicoded);
		GlobalFree(unicoded);
	}
	CloseHandle(hInfo);//g00wno niestabilne w hooy ale wy�wietla
}