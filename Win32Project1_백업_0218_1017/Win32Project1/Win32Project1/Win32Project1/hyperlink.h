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
	DestroyWindow(hwndStatic); //한번 없애주고 시작

	/* 하이퍼링크 부분 : 어디든지 빼서 만들 수 있다.*/
	mycur = LoadCursorFromFile("hlink.cur");
	//Imagelink_text = CreateWindow("static", "서버로부터 도착한 이미지 입니다.", WS_CHILD | WS_VISIBLE, 10, 50, 200, 20, WinProc_sub, 0, g_hInst, 0);
	//SendMessage(Imagelink_text, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hwndStatic = CreateWindow("static", hypername, SS_NOTIFY | WS_CHILD | WS_VISIBLE, 10, 70, 200, 20, WinProc_sub, 0, g_hInst, 0); //이미지 파일 이름으로 하이퍼링크 설정
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
	hypertext = "서버에 이미지가 있습니다.";
	is_hyper_clicked_possible = 1;
}