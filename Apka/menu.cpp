#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <commdlg.h>
#include <iostream>
#include <vector>
#include <cctype>
#include "menu.hpp"
#include "my_huff.hpp"

//TODO, typy i argumenty do pozmieniania
void testMenu(HWND window, LPCWSTR msg) {
	MessageBox(window, msg, L"Menu test", MB_ICONINFORMATION);
}
	
std::wstring string_to_wstring(const std::string& s)
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

void file::menuSaveFile(HWND hwnd) {
	OPENFILENAME file;//TODO Typ *.txt
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = hwnd;
	file.lpstrFilter = L"Pliki tekstowe \0*.txt\0Wszystkie pliki \0*.*\0";
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
	HANDLE hFile = CreateFile((LPCWSTR)file.lpstrFile, FILE_SHARE_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD textboxContentSize = 2 * GetWindowTextLength(hwnd), savedChars;//TOFIX 2 razy wiekszy plik
	LPWSTR textboxContent =(LPWSTR) GlobalAlloc(GPTR, textboxContentSize);
	GetWindowText(hwnd, textboxContent, textboxContentSize);
	if (!WriteFile(hFile, textboxContent, textboxContentSize, &savedChars, NULL))
		MessageBox(hwnd, L"Couldn't save file", L"Error", MB_ICONERROR);
	CloseHandle(hFile);
	GlobalFree(textboxContent);
/* TODO - INTERPRETER(!!!11!)

	std::ifstream badFile(file.lpstrFile);
	std::ofstream goodFile("temporary_name.txt");
	std::string temp;
	char tmp;
	while (!badFile.eof()){//interpreter znaku
		for (int i = 0; i < temp.size(); i++){
			badFile >> tmp;
			if (int(tmp) == 0 || int(tmp) == 1)
				continue;
			switch (tmp)
			{
			case'a': goodFile << 'a'; break;//ma³e litery
			case'b': goodFile << 'b'; break;
			case'c': goodFile << 'c'; break;
			case'd': goodFile << 'd'; break;
			case'e': goodFile << 'e'; break;
			case'f': goodFile << 'f'; break;
			case'g': goodFile << 'g'; break;
			case'h': goodFile << 'h'; break;
			case'i': goodFile << 'i'; break;
			case'j': goodFile << 'j'; break;
			case'k': goodFile << 'k'; break;
			case'l': goodFile << 'l'; break;
			case'm': goodFile << 'm'; break;
			case'n': goodFile << 'n'; break;
			case'o': goodFile << 'o'; break;
			case'p': goodFile << 'p'; break;
			case'q': goodFile << 'q'; break;
			case'r': goodFile << 'r'; break;
			case's': goodFile << 's'; break;
			case't': goodFile << 't'; break;
			case'u': goodFile << 'u'; break;
			case'w': goodFile << 'w'; break;
			case'v': goodFile << 'v'; break;
			case'x': goodFile << 'x'; break;
			case'y': goodFile << 'y'; break;
			case'z': goodFile << 'z'; break;
			case'¹': goodFile << "¹"; break;//polskie
			case'ê': goodFile << "ê"; break;
			case'¿': goodFile << "¿"; break;
			case'Ÿ': goodFile << "Ÿ"; break;
			case'œ': goodFile << "œ"; break;
			case'æ': goodFile << "æ"; break;
			case'ñ': goodFile << "ñ"; break;
			case'³': goodFile << "³"; break;
			case'ó': goodFile << "ó"; break;

			case'A': goodFile << 'A'; break;//du¿e litery
			case'B': goodFile << 'B'; break;
			case'C': goodFile << 'C'; break;
			case'D': goodFile << 'D'; break;
			case'E': goodFile << 'E'; break;
			case'F': goodFile << 'F'; break;
			case'G': goodFile << 'G'; break;
			case'H': goodFile << 'H'; break;
			case'I': goodFile << 'I'; break;
			case'J': goodFile << 'J'; break;
			case'K': goodFile << 'K'; break;
			case'L': goodFile << 'L'; break;
			case'M': goodFile << 'M'; break;
			case'N': goodFile << 'N'; break;
			case'O': goodFile << 'O'; break;
			case'P': goodFile << 'P'; break;
			case'Q': goodFile << 'Q'; break;
			case'R': goodFile << 'R'; break;
			case'S': goodFile << 'S'; break;
			case'T': goodFile << 'T'; break;
			case'U': goodFile << 'U'; break;
			case'W': goodFile << 'W'; break;
			case'V': goodFile << 'V'; break;
			case'X': goodFile << 'X'; break;
			case'Y': goodFile << 'Y'; break;
			case'Z': goodFile << 'Z'; break;
			case'¥': goodFile << '¥'; break;//polskie
			case'Ê': goodFile << 'Ê'; break;
			case'¯': goodFile << '¯'; break;
			case'': goodFile << ''; break;
			case'Œ': goodFile << 'Œ'; break;
			case'Æ': goodFile << 'Æ'; break;
			case'Ñ': goodFile << 'Ñ'; break;
			case'£': goodFile << '£'; break;
			case'Ó': goodFile << 'Ó'; break;

			case'0': goodFile << '0'; break;//liczby
			case'1': goodFile << '1'; break;
			case'2': goodFile << '2'; break;
			case'3': goodFile << '3'; break;
			case'4': goodFile << '4'; break;
			case'5': goodFile << '5'; break;
			case'6': goodFile << '6'; break;
			case'7': goodFile << '7'; break;
			case'8': goodFile << '8'; break;
			case'9': goodFile << '9'; break;

			case ' ': goodFile << ' '; break;//znaki specjalne
			case ',': goodFile << ','; break;
			case '!': goodFile << '!'; break;
			case '?': goodFile << '?'; break;
			case '.': goodFile << '.'; break;
			case ':': goodFile << ':'; break;
			case ';': goodFile << ';'; break;
			case '@': goodFile << '@'; break;
			case '#': goodFile << '#'; break;
			case '$': goodFile << '$'; break;
			case '%': goodFile << '%'; break;
			case '^': goodFile << '^'; break;
			case '&': goodFile << '&'; break;
			case '*': goodFile << '*'; break;
			case '(': goodFile << '('; break;
			case ')': goodFile << ')'; break;
			case '-': goodFile << '-'; break;
			case '_': goodFile << '_'; break;
			case '=': goodFile << '='; break;
			case '+': goodFile << '+'; break;
			case '\\': goodFile << '\\'; break;
			case '/': goodFile << '/'; break;
			case '|': goodFile << '|'; break;
			case '\t': goodFile << '\t'; break;
			case '\n': goodFile << '\n'; break;
			case '\r': goodFile << '\r'; break;
			case'\r\n': goodFile << '\r\n'; break;
			case '\0': goodFile << '\0'; break;
			case '<': goodFile << '<'; break;
			case '>': goodFile << '>'; break;
			case '\'': goodFile << '\''; break;
			case '"': goodFile << '"'; break;
			case '{': goodFile << '{'; break;
			case '}': goodFile << '}'; break;
			case '[': goodFile << '['; break;
			case ']': goodFile << ']'; break;
			case '~': goodFile << '~'; break;
			case '`': goodFile << '`'; break;

			default: break;
			}

		}
	}
	badFile.close();
	goodFile.close();
	DeleteFileA((LPCSTR)file.lpstrFile);
	LPCSTR name = "temporary_name.txt";
	//MoveFile((LPCWSTR)name, (LPCTSTR)file.lpstrFile);
	//DeleteFileA(name);
	*/
}

void file::menuOpen(HWND hwnd)
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
			dwSize = 2 * GetFileSize(hFile, NULL);
		if (dwSize == 0xFFFFFFFF)
			MessageBox(hwnd, L"File size extending 4GB", L"Error", MB_ICONERROR);
		else {
			buff =(LPSTR) GlobalAlloc(GPTR, dwSize);
			if (!ReadFile(hFile, buff, dwSize, &dwTmpBuff, NULL))//TOFIX czasem ucina niezaleznie od rozmiaru pliku, a czasem wyswietla cale 470KB tekstu, ale stabilne toto
				MessageBox(hwnd, L"File reading failure", L"Error", MB_ICONERROR);
			else {
				unicoded = (LPSTR)GlobalAlloc(GPTR, dwSize);
				MultiByteToWideChar(CP_ACP, 0, buff, -1, (LPWSTR)unicoded, dwSize);
				unicoded[dwSize-1] = 0;
				SetWindowText(hwnd,(LPCWSTR) unicoded);
				GlobalFree(unicoded);
			}
			GlobalFree(buff);
			CloseHandle(hFile);
		}		
	}	
}

void file::menuCompressLZW(HWND hwnd) {
	testMenu(hwnd, L"Skompresuj¿e LZW i eksportuj Bóg zap³aæ");
}

void file::menuCompressHuffman(HWND hwnd) {
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"Pliki tekstowe \0*.txt\0Wszystkie pliki \0*.*\0";
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = (LPWSTR)fileName;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = L"Kompresuj";
	ofn.lpstrDefExt = L"txt";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXTENSIONDIFFERENT;
	GetOpenFileNameW(&ofn);
	std::ifstream file(ofn.lpstrFile);
	std::fstream res("kompresja_huff_wynik.txt");
	huffman_comp huff;
	huff.compress(file, res);
	res.close();
	file.close();
}

void menuJPEG(HWND hwnd, LPCWSTR wndClassName) {
	//testMenu(hwnd, L"HAHA NIE DLA PSA JPEG");
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

void clearTextbox(HWND hTextbox)
{
	ShowWindow(hTextbox, SW_SHOW);
	SetWindowText(hTextbox, L"");
}