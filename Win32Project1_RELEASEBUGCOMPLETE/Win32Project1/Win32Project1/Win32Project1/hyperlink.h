#pragma once

#include "header.h"

HWND hwndStatic;
HWND WinProc_sub;
HWND Imagelink_text;

INT colormsg = 0;
char * hypertext = NULL;
int is_hyper_clicked_possible = 0;

void image_hyperlink_maker(char * hypername) {
	DestroyWindow(hwndStatic); //�ѹ� �����ְ� ����

	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = "������ �̹����� �ֽ��ϴ�.";
	is_hyper_clicked_possible = 1;
}


void hyperlink_maker(char * hypername) {
	DestroyWindow(hwndStatic); //�ѹ� �����ְ� ����

	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = "�Ʒ� �������� ã���ó���??";
	ZeroMemory(textbox_buffer, sizeof(textbox_buffer));
	strncpy(textbox_buffer, hypername ,strlen(hypername)-1); //����Ű ������ �����
	is_hyper_clicked_possible = 2; //�ѱ� ������ �� ��
}