#pragma once

#include "header.h"

HWND hwndStatic;
HWND WinProc_sub;
HWND Imagelink_text;

INT colormsg = 0;
wchar_t * hypertext = NULL;
int is_hyper_clicked_possible = 0;

void image_hyperlink_maker(wchar_t * hypername) {
	DestroyWindow(hwndStatic); //�ѹ� �����ְ� ����

	hwndStatic = CreateWindow(TEXT("static"), hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 49, 65, 200, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = TEXT("������ �̹����� �ֽ��ϴ�.");
	is_hyper_clicked_possible = 1;
}


void hyperlink_maker(char * hypername) {
	wchar_t * trans_hyper = new wchar_t[strlen(hypername)*sizeof(TCHAR)];
	MultiByteToWideChar(CP_UTF8, 0, hypername, -1, trans_hyper, strlen(hypername)*sizeof(TCHAR));
	DestroyWindow(hwndStatic); //�ѹ� �����ְ� ����

	hwndStatic = CreateWindow(TEXT("static"), trans_hyper, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 49, 65, 200, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = TEXT("�Ʒ� �������� ã���ó���??");
	ZeroMemory(textbox_buffer, sizeof(textbox_buffer));
	wcsncpy(textbox_buffer, trans_hyper,wcslen(trans_hyper)-1); //����Ű ������ �����
	is_hyper_clicked_possible = 2; //�ѱ� ������ �� ��
	delete[] trans_hyper; //�޸� ����
}


void error_hyperlink_maker() {
	DestroyWindow(hwndStatic); //�ѹ� �����ְ� ����
	hwndStatic = CreateWindow(TEXT("static"), TEXT("help.sangjae.co.kr"), SS_NOTIFY | WS_CHILD | WS_VISIBLE, 49, 275, 200, 20, WinProc_sub, 0, g_hInst, 0); //�̹��� ���� �̸����� �����۸�ũ ����
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
}