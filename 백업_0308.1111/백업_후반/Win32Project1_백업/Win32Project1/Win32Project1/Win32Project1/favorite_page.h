#pragma once
#include "header.h"

void favorite_page(wchar_t page[], int len, HWND hWnd) {
	
	if (len <= 0) {
		MessageBox(NULL, TEXT("���������Դϴ�!"), TEXT("����!"), MB_ICONINFORMATION);
		return;
	}
	else if (visit_pagenumber == 50012) {
		MessageBox(NULL, TEXT("���ã�� �ִ� ������ �ʰ��߽��ϴ�!"), TEXT("����!"), MB_ICONINFORMATION);
		return;
	}

	FILE *fp2 = fopen("favorite_page.txt", "r");
		

	if (fp2 != NULL) {
		wchar_t strTemp[255] = { 0 };
		while (!feof(fp2))
		{
			fgetws(strTemp, sizeof(strTemp), fp2);
			if (!wcsncmp(page, strTemp, len)) {
				wcout << strTemp << endl;
				MessageBox(NULL, TEXT("�̹� ��ϵ� ������ �Դϴ�!"), TEXT("����!"), MB_ICONINFORMATION);
				fclose(fp2);
				return;
			}
		}
		fclose(fp2);
	}

	FILE *fp = fopen("favorite_page.txt", "a");
	
	/* WIDE CHARACTER 2BYTE ERROR */
	char* pTemp = NULL;
	int iLen = WideCharToMultiByte(CP_ACP, 0, page, -1, pTemp, 0, NULL, NULL);
	pTemp = new char[iLen + 1];
	WideCharToMultiByte(CP_ACP, 0, page, -1, pTemp, iLen, NULL, NULL);


	string bufs = to_string(visit_pagenumber);
	char *num_temp = const_cast<char*>(bufs.c_str());
	fwrite(pTemp, iLen-1, 1, fp);
	fwrite("@", 1, 1, fp);
	fwrite(num_temp, strlen(num_temp), 1, fp);
	fwrite("\n", 1, 1, fp);
	fclose(fp);

	//www.naver.com@50001\n �������� �� ���̴�.

	MessageBox(NULL, TEXT("���ã�⿡ ��ϵǾ����ϴ�!"), TEXT("���ã�� ��� ����!"), MB_ICONINFORMATION);
	AppendMenu(hPopup_favorite, MF_STRING, visit_pagenumber, page);
	//SetMenu(hWnd, hMenu_favorite);
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

				
		wchar_t strTemp[255] = { 0 };
		while (!feof(fp2))
		{
			memset(strTemp, 0x00, sizeof(strTemp));
			fgetws(strTemp, sizeof(strTemp), fp2);
			if (wcslen(strTemp) > 0) {
				wcstok(strTemp, TEXT("@"));
				wcout << strTemp << endl;
				AppendMenu(hPopup_favorite, MF_STRING, visit_pagenumber, strTemp);
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

	wstring temps = to_wstring(search_id);
	wchar_t *num_temp = const_cast<wchar_t*>(temps.c_str());

	if (fp2 != NULL) {

		fseek(fp2, 0, SEEK_END);
		int textlen = ftell(fp2);

		fseek(fp2, 0, SEEK_SET); // ������ ��ġ �ٽ� ó������ ����

		wchar_t strTemp[50] = { 0 };
		while (!feof(fp2))
		{
			memset(strTemp, 0x00, sizeof(strTemp));
			fgetws(strTemp, sizeof(strTemp), fp2);
			if (wcslen(strTemp) > 0) {
				wcstok(strTemp, TEXT("@"));
				if (!wcsncmp(num_temp, (wcstok(NULL, TEXT("@"))), 5)) {
					wcout <<TEXT("���ã�� ��û���� �ּ� :") << strTemp  << endl;
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

void Save_Home() {

	char* pTemp = NULL;
	int iLen = WideCharToMultiByte(CP_ACP, 0, textbox_buffer, -1, pTemp, 0, NULL, NULL);
	pTemp = new char[iLen + 1];
	WideCharToMultiByte(CP_ACP, 0, textbox_buffer, -1, pTemp, iLen, NULL, NULL);

	FILE * fp = fopen("home_save.txt", "w");

	fwrite(pTemp, iLen, 1, fp);

	fclose(fp);
	MessageBox(NULL, TEXT("���� �������� ���� �Ǿ����ϴ�!"), TEXT("���������� ����!"), MB_ICONINFORMATION);
}


void load_Home() {
	FILE * fp = fopen("home_save.txt", "r");

	wchar_t strTemp[50] = { 0 };
	memset(strTemp, 0x00, sizeof(strTemp));
	fgetws(strTemp, sizeof(strTemp), fp);
	
	fclose(fp);


	if (input_valid_check(strTemp) == 1) // �ּ�üũ�ϰ� dns ������������ ����
		Save_visit_page(strTemp);
	InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
	UpdateWindow(Main_hWnd);
	SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)strTemp);
	++combo_page_count;
}
