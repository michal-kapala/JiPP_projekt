#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <commdlg.h>
#include <iostream>
#include <vector>
#include "menu.hpp"

//TODO, typy i argumenty do pozmieniania
void testMenu(HWND window, LPCWSTR msg) {
	MessageBox(window, msg, L"Menu test", MB_ICONINFORMATION);
}
	
std::wstring string_to_wstring(const std::string& s)//konwersja - https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void file::menuNewFile(HWND hwnd) {
	//testMenu(hwnd, L"Zrób¿e nowy plik");
	SetWindowText(hwnd, L"");

	OPENFILENAME file;//TODO Typ *.txt
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = hwnd;
	file.lpstrFilter = NULL;
	char fileBuffer[MAX_PATH] = "";
	file.lpstrFile = (LPWSTR)fileBuffer;
	file.nMaxFile = MAX_PATH;
	file.lpstrInitialDir = NULL;
	file.lpstrTitle = L"Zapisz jako";
	file.lpstrDefExt = L"txt";
	file.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	
	if (!GetSaveFileNameW(&file)) {
		switch (CommDlgExtendedError()) {
		case 0xFFFF:
			MessageBox(hwnd, L"DialogBox function failed (check window handle)", L"Error", MB_ICONERROR);
			break;
		case 0x0006: 
			MessageBox(hwnd, L"Specified resource not found", L"Error", MB_ICONERROR);
			break;
		case 0x0002:
			MessageBox(hwnd, L"Initialization failed (insufficient memory)", L"Error", MB_ICONERROR);
			break;
		case 0x0007: 
			MessageBox(hwnd, L"Loading the specified resource failed", L"Error", MB_ICONERROR);
			break;
		case 0x0005: 
			MessageBox(hwnd, L"Loading the specified string failed", L"Error", MB_ICONERROR);
			break;
		case 0x0001:
			MessageBox(hwnd, L"IStructSize member of the common dialog box is invalid", L"Error", MB_ICONERROR);
			break;
		default: 
			MessageBox(hwnd, L"Check MSDN CDERR_ list", L"Error", MB_ICONERROR);
			break;
		}
	}
	HANDLE hFile = CreateFile((LPCWSTR)file.lpstrFile, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFile);
}

void file::menuOpenView(HWND hwnd)
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"Pliki tekstowe \0*.txt\0Wszystkie pliki \0*.*\0";
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = (LPWSTR) fileName;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = L"Otwórz";
	ofn.lpstrDefExt = L"txt";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXTENSIONDIFFERENT;
	GetOpenFileNameW(&ofn);

	HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dwSize, dwTmpBuff;
	LPSTR buff, unicoded;
	
	if (hFile == INVALID_HANDLE_VALUE)
		MessageBox(hwnd, L"File opening failure", L"Error", MB_ICONERROR);
	else {
			dwSize = GetFileSize(hFile, NULL);
		if (dwSize == 0xFFFFFFFF)
			MessageBox(hwnd, L"File size extending 4GB", L"Error", MB_ICONERROR);
		else {
			buff =(LPSTR) GlobalAlloc(GPTR, dwSize +1);
			if (!ReadFile(hFile, buff, dwSize, &dwTmpBuff, NULL))//TOFIX czasem ucina niezaleznie od rozmiaru pliku, a czasem wyswietla cale 470KB tekstu, ale stabilne toto
				MessageBox(hwnd, L"File reading failure", L"Error", MB_ICONERROR);
			else {
				unicoded = (LPSTR)GlobalAlloc(GPTR, BUFFER_MAX_SIZE+1);
				MultiByteToWideChar(CP_ACP, 0, buff, -1, (LPWSTR)unicoded, BUFFER_MAX_SIZE+1);
				unicoded[dwSize] = 0;
				SetWindowText(hwnd,(LPCWSTR) unicoded);
				GlobalFree(unicoded);
			}
			GlobalFree(buff);
			CloseHandle(hFile);
		}		
	}	
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

void menuInfo(HWND hTextbox) {
	
	std::ifstream info(INFO_ID);
	std::string chain, buffer;
	if (!info)
		MessageBox(hTextbox, L"File opening failure. Check directory.", L"Error", MB_ICONERROR);
	else{
		while (!info.eof()) {
			std::getline(info, buffer);
			chain.append(buffer);//string stress test kappa
			chain += "\r\n";//kiedy siedzisz nad wyswietlaniem enterkow 3h po to zeby uswiadomic sobie ze koniec wiersza w windowsie to nie \n tylko \r\n
		}
		
		std::wstring stemp = string_to_wstring(chain);
		LPCWSTR result = stemp.c_str();
		SetWindowText(hTextbox, result);
	}
	info.close();
}