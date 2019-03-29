#include "stdafx.h"
#include <windows.h>
#include "resource.h"

void testMenu(HWND window, LPCWSTR msg){
	MessageBox(window, msg, L"Ja kurwa nie wiem czemu jestem taki zajebisty", MB_ICONINFORMATION);
}

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

	if (!RegisterClassEx(&mainClass)) {
		MessageBox(NULL, L"Window class registration denial", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(1));//MENU_ID

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
	case WM_COMMAND: //TODO, wyjątki, jakoś trzeba ogarnąć te identyfikatory z menu.rc
		if (LOWORD(wParam) == 10){
			testMenu(hwnd, L"Zróbże nowy plik");
			break;
		}	
		if (LOWORD(wParam) == 11){
			testMenu(hwnd, L"Otwórzże podgląd pliku źródłowego");
			break;
		}
		if (LOWORD(wParam) == 12) {
			testMenu(hwnd, L"Skompresujże LZW i wyświetl łaskawie");
			break;
		}
		if (LOWORD(wParam) == 13) {
			testMenu(hwnd, L"Skompresujże Huffmankiem i wyświetl łaskawie");
			break;
		}
		if (LOWORD(wParam) == 14) {
			testMenu(hwnd, L"Masz poczęstuj się edycją");
			break;
		}
		if (LOWORD(wParam) == 15) {
			testMenu(hwnd, L"Skompresujże LZW i eksportuj Bóg zapłać");
			break;
		}
		if (LOWORD(wParam) == 16) {
			testMenu(hwnd, L"Skompresujże Huffmankiem i eksportuj tak jak Pan Jezus powiedział");
			break;
		}
		if (LOWORD(wParam) == 17) {
			testMenu(hwnd, L"HAHA NIE DLA PSA JPEG");
			break;
		}
		if (LOWORD(wParam) == 18) {
			testMenu(hwnd, L"Nie no żartowałem, masz, poczęstuj się pomocą");
			break;
		}//koniec TODO WM_COMMANDA
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