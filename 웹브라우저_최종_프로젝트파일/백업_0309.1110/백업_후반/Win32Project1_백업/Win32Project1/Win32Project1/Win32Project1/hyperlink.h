#pragma once

#include "header.h"

HWND hwndStatic;
HWND WinProc_sub;
HWND Imagelink_text;

INT colormsg = 0;
wchar_t * hypertext = NULL;
int is_hyper_clicked_possible = 0;

void image_hyperlink_maker(wchar_t * hypername) {
	DestroyWindow(hwndStatic); //한번 없애주고 시작

	hwndStatic = CreateWindow(TEXT("static"), hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 49, 65, 200, 20, WinProc_sub, 0, g_hInst, 0); //이미지 파일 이름으로 하이퍼링크 설정
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = TEXT("서버에 이미지가 있습니다.");
	is_hyper_clicked_possible = 1;
}


void hyperlink_maker(char * hypername) {
	wchar_t * trans_hyper = new wchar_t[strlen(hypername)*sizeof(TCHAR)];
	MultiByteToWideChar(CP_UTF8, 0, hypername, -1, trans_hyper, strlen(hypername)*sizeof(TCHAR));
	DestroyWindow(hwndStatic); //한번 없애주고 시작

	hwndStatic = CreateWindow(TEXT("static"), trans_hyper, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 49, 65, 200, 20, WinProc_sub, 0, g_hInst, 0); //이미지 파일 이름으로 하이퍼링크 설정
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = TEXT("아래 페이지를 찾으시나요??");
	ZeroMemory(textbox_buffer, sizeof(textbox_buffer));
	wcsncpy(textbox_buffer, trans_hyper,wcslen(trans_hyper)-1); //엔터키 제거해 줘야함
	is_hyper_clicked_possible = 2; //한글 페이지 일 때
	delete[] trans_hyper; //메모리 제거
}


void error_hyperlink_maker() {
	DestroyWindow(hwndStatic); //한번 없애주고 시작
	hwndStatic = CreateWindow(TEXT("static"), TEXT("help.sangjae.co.kr"), SS_NOTIFY | WS_CHILD | WS_VISIBLE, 49, 275, 200, 20, WinProc_sub, 0, g_hInst, 0); //이미지 파일 이름으로 하이퍼링크 설정
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
}