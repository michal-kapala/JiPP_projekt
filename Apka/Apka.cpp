#include "stdafx.h"
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "menu.hpp"
#include "my_huff.hpp"
#include "JPEG.hpp"

HWND mainWnd, hTextbox;
MSG message;
HMENU hMenu;
LPTSTR defPath;// \Projekt_JiPP\Apka\ //
double quality;

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK jpegDlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { //main
	WNDCLASSEX mainClass;//klasa okna
	LPCWSTR mainClassName = L"Main window's class";
	mainClass.cbSize = sizeof(mainClass);
	mainClass.style = 0;
	mainClass.lpfnWndProc = MainWindowProc;
	mainClass.cbClsExtra = 0;
	mainClass.cbWndExtra = 0;
	mainClass.hInstance = hInstance;
	mainClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(POGGERS));
	mainClass.hCursor = LoadCursorFromFile(L"cursor.cur");
	mainClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1);
	mainClass.lpszMenuName = (LPCWSTR) hMenu;
	mainClass.lpszClassName = mainClassName;
	mainClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(POGGERS));

	if (!RegisterClassEx(&mainClass)) {//error
		MessageBox(NULL, L"Window class registration denial", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MENU_ID));

	mainWnd = CreateWindowEx(WS_EX_CLIENTEDGE, mainClassName, L"Expression: vector subscript out of range", WS_OVERLAPPEDWINDOW | WS_SIZEBOX,//okno glowne
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 405, NULL, hMenu, hInstance, NULL);

	HBRUSH brush = CreateSolidBrush(RGB(170, 210, 255));//niebieski
	SetClassLongPtr(mainWnd, GCLP_HBRBACKGROUND, (LONG)brush);

	hTextbox = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |//glowny textbox
		WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, -2, -2, 584, 350, mainWnd, NULL, hInstance, NULL);

	defPath = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH);
	GetCurrentDirectoryW(MAX_PATH, defPath);

	ShowWindow(mainWnd, nCmdShow);
	UpdateWindow(mainWnd);

	while (GetMessage(&message, NULL, 0, 0)) {//petla komunikatow
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
	GlobalFree(defPath);
	return 0;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	SetCurrentDirectory(defPath);
	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)){//menu
		case MENU_FILE_SAVE:
			file::menuSaveFile(hTextbox);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_OPEN:
			clearTextbox(hTextbox);
			file::menuOpen(hTextbox);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_COMPRESS_LZW:
			ShowWindow(hTextbox, SW_SHOW);
			file::menuCompressLZW(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_DECOMPRESS_LZW:
			ShowWindow(hTextbox, SW_SHOW);
			file::menuDecompressLZW(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_COMPRESS_HUFFMAN:
			ShowWindow(hTextbox, SW_SHOW);
			file::menuCompressHuffman(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_DECOMPRESS_HUFFMAN:
			ShowWindow(hTextbox, SW_SHOW);
			file::menuDecompressHuffman(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_JPEG:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(DIALOG_TITLE), hwnd, (DLGPROC)jpegDlgProc);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_INFO:	
			clearTextbox(hTextbox);
			menuInfo(hTextbox);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

BOOL CALLBACK jpegDlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case QUALITY_25:
			if (IsDlgButtonChecked(hwnd, QUALITY_25) == BST_UNCHECKED){
				quality = 25;
				CheckDlgButton(hwnd, QUALITY_25, BST_CHECKED);
				CheckDlgButton(hwnd, QUALITY_50, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_75, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_95, BST_UNCHECKED);
			}
			else {
				CheckDlgButton(hwnd, QUALITY_25, BST_UNCHECKED);
				quality = 50;
				CheckDlgButton(hwnd, QUALITY_50, BST_CHECKED);
			}
			break;
		case QUALITY_50:
			if (IsDlgButtonChecked(hwnd, QUALITY_50) == BST_UNCHECKED) {
				quality = 50;
				CheckDlgButton(hwnd, QUALITY_25, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_50, BST_CHECKED);
				CheckDlgButton(hwnd, QUALITY_75, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_95, BST_UNCHECKED);
			}
			else {
				CheckDlgButton(hwnd, QUALITY_50, BST_UNCHECKED);
				quality = 50;
			}	
			break;
		case QUALITY_75:
			if (IsDlgButtonChecked(hwnd, QUALITY_75) == BST_UNCHECKED) {
				quality = 75;
				CheckDlgButton(hwnd, QUALITY_25, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_50, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_75, BST_CHECKED);
				CheckDlgButton(hwnd, QUALITY_95, BST_UNCHECKED);
			}
			else {
				CheckDlgButton(hwnd, QUALITY_75, BST_UNCHECKED);
				quality = 50;
				CheckDlgButton(hwnd, QUALITY_50, BST_CHECKED);
			}
			break;
		case QUALITY_95:
			if (IsDlgButtonChecked(hwnd, QUALITY_95) == BST_UNCHECKED) {
				quality = 95;
				CheckDlgButton(hwnd, QUALITY_25, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_50, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_75, BST_UNCHECKED);
				CheckDlgButton(hwnd, QUALITY_95, BST_CHECKED);
			}
			else
			{
				CheckDlgButton(hwnd, QUALITY_95, BST_UNCHECKED);
				quality = 50;
				CheckDlgButton(hwnd, QUALITY_50, BST_CHECKED);
			}
			break;
		case DIALOG_COMPRESS:
			if (quality != 25 && quality != 50 && quality != 75 && quality != 95)
				quality = 50;
			rysuj_8x8(hwnd, quality);
			break;
		case DIALOG_SHOW_EXAMPLE:
			if (quality != 25 && quality != 50 && quality != 75 && quality != 95)
				quality = 50;
			rysuj_przyklad(hwnd, quality);
			break;
		case WM_DESTROY:
			EndDialog(hwnd, DIALOG_COMPRESS);
			break;
		}
	}
	break;

	default: return FALSE;
	}

	return TRUE;
}