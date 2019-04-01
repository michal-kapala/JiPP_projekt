#pragma once

#define MENU_ID                         1
#define MENU_FILE_NEW                   10
#define MENU_FILE_OPEN_VIEW             11
#define MENU_FILE_OPEN_COMPVIEW_LZW     12
#define MENU_FILE_OPEN_COMPVIEW_HUFFMAN 13
#define MENU_FILE_EDIT                  14
#define MENU_FILE_COMPRESS_LZW          15
#define MENU_FILE_COMPRESS_HUFFMAN      16
#define MENU_JPEG                       17
#define MENU_HELP                       18

void testMenu(HWND window, LPCWSTR msg) {
	MessageBox(window, msg, L"Ja kurwa nie wiem czemu jestem taki zajebisty", MB_ICONINFORMATION);
}