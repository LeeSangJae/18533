#pragma once
#include "header.h"

void favorite_page(char page[], int len, HWND hWnd) {
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
		char * pStr = NULL;
		char strTemp[255] = { 0 };
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
		fclose(fp2);
	}

	FILE *fp = fopen("favorite_page.txt", "a");
	
	string bufs = to_string(visit_pagenumber);
	char *num_temp = const_cast<char*>(bufs.c_str());
	fwrite(page, len, 1, fp);
	fwrite("@", 1, 1, fp);
	fwrite(num_temp, strlen(num_temp), 1, fp);
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

	string temps = to_string(search_id);
	char *num_temp = const_cast<char*>(temps.c_str());

	if (fp2 != NULL) {

		fseek(fp2, 0, SEEK_END);
		int textlen = ftell(fp2);

		fseek(fp2, 0, SEEK_SET); // ������ ��ġ �ٽ� ó������ ����

		char strTemp[50] = { 0 };
		while (!feof(fp2))
		{
			memset(strTemp, 0x00, sizeof(strTemp));
			fgets(strTemp, sizeof(strTemp), fp2);
			if (strlen(strTemp) > 0) {
				strtok(strTemp, "@");
				if (!strncmp(num_temp, (strtok(NULL, "@")), 5)) {
					printf("���ã�� ��û���� �ּ� : %s\n", strTemp);
					memset(image_file_name, 0x00, sizeof(image_file_name));
					if (input_valid_check(strTemp) == 1) // �ּ�üũ�ϰ� dns ������������ ���� string���� �Ѱ���� �ɵ�
						Save_visit_page(strTemp);
					InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
					UpdateWindow(Main_hWnd);
					break;
				}
			}
		}
		fclose(fp2);
	}
}