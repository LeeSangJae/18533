#pragma once
#include "header.h"

void favorite_page(char * page, int len, HWND hWnd) {
	if (len <= 0) {
		MessageBox(NULL, "빈페이지입니다!", "에러!", MB_ICONINFORMATION);
		return;
	}
	else if (visit_pagenumber == 50012) {
		MessageBox(NULL, "즐겨찾기 최대 개수를 초과했습니다!", "에러!", MB_ICONINFORMATION);
		return;
	}

	FILE *fp2 = fopen("favorite_page.txt", "r");
		

	if (fp2 != NULL) {
		char * pStr;
		char strTemp[255];
		while (!feof(fp2))
		{
			pStr = fgets(strTemp, sizeof(strTemp), fp2);
			if (!strncmp(page, strTemp, len)) {
				printf("%s", strTemp);
				MessageBox(NULL, "이미 등록된 페이지 입니다!", "에러!", MB_ICONINFORMATION);
				fclose(fp2);
				return;
			}
		}
	}

	FILE *fp = fopen("favorite_page.txt", "a");
	
	//fseek(fp, 0, SEEK_END);
	char buf[6] = { 0 };
	itoa(visit_pagenumber, buf, 6);
	fwrite(page, len, 1, fp);
	fwrite("@", 1, 1, fp);
	fwrite(buf, 5, 1, fp);
	fwrite("\n", 1, 1, fp);
	fclose(fp);

	//www.naver.com@50001\n 형식으로 될 것이다.


	MessageBox(NULL, "즐겨찾기에 등록되었습니다!", "즐겨찾기 등록 성공!", MB_ICONINFORMATION);
	AppendMenu(hPopup, MF_STRING, visit_pagenumber, page);
	SetMenu(hWnd, hMenu);
	++visit_pagenumber;
}

void favorite_page_create(HWND hWnd) {
	FILE *fp2 = NULL;

	if (!(fp2 = fopen("favorite_page.txt", "r"))) {
		return;
	}

	if (fp2 != NULL) {
		
		fseek(fp2, 0, SEEK_END);
		int textlen = ftell(fp2);

		//totalresult = new char[textlen];
		//memset(totalresult, 0x00, sizeof(totalresult)); //메모리 초기화 안하면 쓰레기값 찍힌다.
		fseek(fp2, 0, SEEK_SET); // 오프셋 위치 다시 처음으로 설정

				
		char strTemp[255] = { 0 };
		while (!feof(fp2))
		{
			memset(strTemp, 0x00, sizeof(strTemp));
			fgets(strTemp, sizeof(strTemp), fp2);
			if (strlen(strTemp) > 0) {
				strtok(strTemp, "@");
				printf("%s\n", strTemp);
				AppendMenu(hPopup, MF_STRING, visit_pagenumber, strTemp);
				SetMenu(hWnd, hMenu);
				++visit_pagenumber;
				//strncat(totalresult, strTemp, strlen(strTemp));
			}
		}
		fclose(fp2);
	}
}

void favorite_clicked(int search_id)
{
	FILE *fp2 = NULL;

	if (!(fp2 = fopen("favorite_page.txt", "r"))) {
		return;
	}

	char buf[6] = { 0 };
	itoa(search_id, buf, 10);

	if (fp2 != NULL) {

		fseek(fp2, 0, SEEK_END);
		int textlen = ftell(fp2);

		//totalresult = new char[textlen];
		//memset(totalresult, 0x00, sizeof(totalresult)); //메모리 초기화 안하면 쓰레기값 찍힌다.
		fseek(fp2, 0, SEEK_SET); // 오프셋 위치 다시 처음으로 설정

		
		char strTemp[50] = { 0 };
		while (!feof(fp2))
		{
			memset(strTemp, 0x00, sizeof(strTemp));
			fgets(strTemp, sizeof(strTemp), fp2);
			if (strlen(strTemp) > 0) {
				strtok(strTemp, "@");
				if (!strncmp(buf, (strtok(NULL, "@")), 5)) {
					printf("즐겨찾기 요청중인 주소 : %s\n", strTemp);
					input_valid_check(strTemp); // 주소체크하고 dns 실행할지말지 결정
					InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
					UpdateWindow(Main_hWnd);
					break;
				}
			}
		}
		fclose(fp2);
	}
}