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
LPTSTR defPath;// \Projekt_JiPP\Apka\

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
	mainClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//TODO
	mainClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mainClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1);
	mainClass.lpszMenuName = (LPCWSTR) hMenu;
	mainClass.lpszClassName = mainClassName;
	mainClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//TODO

	if (!RegisterClassEx(&mainClass)) {//error
		MessageBox(NULL, L"Window class registration denial", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MENU_ID));

	mainWnd = CreateWindowEx(WS_EX_CLIENTEDGE, mainClassName, L"Nazwa aplikacji", WS_OVERLAPPEDWINDOW | WS_SIZEBOX,//okno glowne
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
	case WM_COMMAND: //TODO, wyjątki, funkcje w menu.hpp
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
		case MENU_FILE_COMPRESS_HUFFMAN:
			ShowWindow(hTextbox, SW_SHOW);
			file::menuCompressHuffman(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_JPEG://!!!!!!!!!!!!!!11111111111!!!!!!!!!11!!!1!!1!1111111111111!!11!!!!111!1!!!!!!!!!!!!!1111!11111!!!111!!!!!1!1!!!!
			ShowWindow(hTextbox, SW_HIDE);
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
		// reakcja na przyciski
		switch (LOWORD(wParam))
		{
		case DIALOG_COMPRESS:
			rysuj_8x8(hwnd);
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