#pragma once

#include "header.h"

HWND hwndStatic;
HWND WinProc_sub;
HWND Imagelink_text;

INT colormsg = 0;
HCURSOR mycur;
char * hypertext = NULL;
int is_hyper_clicked_possible = 0;

void image_hyperlink_maker(char * hypername) {
	DestroyWindow(hwndStatic); //�ѹ� �����ְ� ����

	/* �����۸�ũ �κ� : ������ ���� ���� �� �ִ�.*/
	mycur = LoadCursorFromFile("hlink.cur");
	//Imagelink_text = CreateWindow("static", "�����κ��� ������ �̹��� �Դϴ�.", WS_CHILD | WS_VISIBLE, 10, 50, 200, 20, WinProc_sub, 0, g_hInst, 0);
	//SendMessage(Imagelink_text, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = "������ �̹����� �ֽ��ϴ�.";
	is_hyper_clicked_possible = 1;
}