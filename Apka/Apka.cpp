﻿#include "stdafx.h"
#include <windows.h>
#include "menu.hpp"

MSG message;
HMENU hMenu;

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	mainClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	mainClass.lpszMenuName = (LPCWSTR) hMenu;
	mainClass.lpszClassName = mainClassName;
	mainClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//TODO

	if (!RegisterClassEx(&mainClass)) {//error
		MessageBox(NULL, L"Window class registration denial", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MENU_ID));

	HWND mainWnd = CreateWindowEx(WS_EX_CLIENTEDGE, mainClassName, L"Nazwa aplikacji", WS_OVERLAPPEDWINDOW | WS_SIZEBOX,//okno glowne
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, hMenu, hInstance, NULL);

	ShowWindow(mainWnd, nCmdShow);
	UpdateWindow(mainWnd);

	while (GetMessage(&message, NULL, 0, 0)) {//petla komunikatow
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;

	return 0;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: //TODO, wyjątki, funkcje w menu.hpp
		switch (LOWORD(wParam)){//menu
		case MENU_FILE_NEW:
			file::menuNewFile(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_OPEN_VIEW:
			file::menuOpenView(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_OPEN_COMPVIEW_LZW:
			file::menuOpenCompViewLZW(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_OPEN_COMPVIEW_HUFFMAN:
			file::menuOpenCompViewHuffman(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_EDIT:
			file::menuEdit(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_COMPRESS_LZW:
			file::menuCompressLZW(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_FILE_COMPRESS_HUFFMAN:
			file::menuCompressHuffman(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_JPEG:
			menuJPEG(hwnd);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		case MENU_HELP:
			menuHelp(hwnd);
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