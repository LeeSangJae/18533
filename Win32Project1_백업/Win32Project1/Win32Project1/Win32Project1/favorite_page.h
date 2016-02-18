#pragma once
#include "header.h"

void favorite_page(char * page, int len, HWND hWnd) {
	if (len <= 0) {
		MessageBox(NULL, "���������Դϴ�!", "����!", MB_ICONINFORMATION);
		return;
	}
	else if (visit_pagenumber == 50012) {
		MessageBox(NULL, "���ã�� �ִ� ������ �ʰ��߽��ϴ�!", "����!", MB_ICONINFORMATION);
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
				MessageBox(NULL, "�̹� ��ϵ� ������ �Դϴ�!", "����!", MB_ICONINFORMATION);
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

	//www.naver.com@50001\n �������� �� ���̴�.


	MessageBox(NULL, "���ã�⿡ ��ϵǾ����ϴ�!", "���ã�� ��� ����!", MB_ICONINFORMATION);
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
		//memset(totalresult, 0x00, sizeof(totalresult)); //�޸� �ʱ�ȭ ���ϸ� �����Ⱚ ������.
		fseek(fp2, 0, SEEK_SET); // ������ ��ġ �ٽ� ó������ ����

				
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
		//memset(totalresult, 0x00, sizeof(totalresult)); //�޸� �ʱ�ȭ ���ϸ� �����Ⱚ ������.
		fseek(fp2, 0, SEEK_SET); // ������ ��ġ �ٽ� ó������ ����

		
		char strTemp[50] = { 0 };
		while (!feof(fp2))
		{
			memset(strTemp, 0x00, sizeof(strTemp));
			fgets(strTemp, sizeof(strTemp), fp2);
			if (strlen(strTemp) > 0) {
				strtok(strTemp, "@");
				if (!strncmp(buf, (strtok(NULL, "@")), 5)) {
					printf("���ã�� ��û���� �ּ� : %s\n", strTemp);
					input_valid_check(strTemp); // �ּ�üũ�ϰ� dns ������������ ����
					InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
					UpdateWindow(Main_hWnd);
					break;
				}
			}
		}
		fclose(fp2);
	}
}