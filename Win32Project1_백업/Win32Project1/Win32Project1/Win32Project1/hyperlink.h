#pragma once

#include "header.h"

HWND hwndStatic;
HWND WinProc_sub;

INT colormsg = 0;
HCURSOR mycur;

void image_hyperlink_maker(char * hypername) {
	/* �����۸�ũ �κ� : ������ ���� ���� �� �ִ�.*/
	mycur = LoadCursorFromFile("hlink.cur");
	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 50, 150, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
}