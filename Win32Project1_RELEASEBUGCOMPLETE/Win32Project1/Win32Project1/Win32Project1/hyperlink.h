#pragma once

#include "header.h"

HWND hwndStatic;
HWND WinProc_sub;
HWND Imagelink_text;

INT colormsg = 0;
char * hypertext = NULL;
int is_hyper_clicked_possible = 0;

void image_hyperlink_maker(char * hypername) {
	DestroyWindow(hwndStatic); //한번 없애주고 시작

	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, WinProc_sub, 0, g_hInst, 0); //이미지 파일 이름으로 하이퍼링크 설정
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = "서버에 이미지가 있습니다.";
	is_hyper_clicked_possible = 1;
}


void hyperlink_maker(char * hypername) {
	DestroyWindow(hwndStatic); //한번 없애주고 시작

	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, WinProc_sub, 0, g_hInst, 0); //이미지 파일 이름으로 하이퍼링크 설정
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = "아래 페이지를 찾으시나요??";
	ZeroMemory(textbox_buffer, sizeof(textbox_buffer));
	strncpy(textbox_buffer, hypername ,strlen(hypername)-1); //엔터키 제거해 줘야함
	is_hyper_clicked_possible = 2; //한글 페이지 일 때
}