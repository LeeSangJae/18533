#pragma once
#include "header.h"
#include "hyperlink.h"

wchar_t image_file_name[100] = { 0 };
int image_exist = 0;
char query_jpg[100] = { 0 };

wstring Search_header(wstring get_str)
{
	wregex pattern(TEXT("(head|HEAD)\(.*)\(head|HEAD)"));
	
	wsmatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(get_str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "��° : " << m[i] << endl;
		}
		return m[2];
	}
	else {
		return TEXT("����� ����");
		wcout << TEXT("����� ��ġ���� ����") << endl;
	}
}

wstring Search_body(wstring get_str)
{
	wregex pattern(TEXT("(body|BODY|script)\(.*)\(body|BODY|script)"));
	wsmatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(get_str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "��° : " << m[i] << endl;
		}
		return m[2];
	}
	else {
		return TEXT("�ٵ� ����");
		wcout << TEXT("�ٵ� ��ġ���� ����") << endl;
	}
}


void Search_jpg_image(wstring get_str)
{
	wchar_t * get_cast = NULL;
	wchar_t * get_cast_type = NULL; //�̹��� Ÿ��
	char * get_cast_type_for_socket = NULL;

	wchar_t * pattern_str = NULL;
	if (temp_port2 == 8090)//��δ�
		pattern_str = TEXT("\"\(.*)\(jpg|bmp|gif|jpeg)\(\"></br>)");
	else if (temp_port2 = 8677)//ȿ�ٴԲ����� bmp�� �޾ƿ���;
		pattern_str = TEXT("\"\(.*)\(bmp)");
	else if (temp_port2 = 8979) //��̴�
		pattern_str = TEXT("\"\(.*)\(jpg|bmp|gif|jpeg)\">");

	if (pattern_str == NULL)
		return;

	wregex pattern(pattern_str);
	wsmatch m;
	ZeroMemory(&m, sizeof(m));
	wchar_t get_jpg[20] = { 0 };
	char jpg_name[20] = { 0 };
	int k = 0;
	if (regex_search(get_str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "��° : " << m[i] << endl;
		}
		wstring temps(m[1].str());
		wstring wstr1;
		wstr1.assign(temps.begin(), temps.end());
		wstring tempstype(m[2].str());
		wstring wstr2;
		wstr2.assign(tempstype.begin(), tempstype.end());
		
		string sstr;
		sstr.assign(wstr2.begin(), wstr2.end());

		get_cast = const_cast<wchar_t*>(wstr1.c_str());
		get_cast_type = const_cast<wchar_t*>(wstr2.c_str());
		get_cast_type_for_socket = const_cast<char*>(sstr.c_str());

		wsprintf(get_jpg, TEXT("%s"), get_cast);
		for (int i = 0; i < wcslen(get_jpg); i++)
		{
			if (!IsCharAlphaNumeric(get_jpg[i])) {
				get_jpg[i] = ' ';
			}
		}
		for (int i = 0; i < wcslen(get_jpg); i++)
		{
			if (!isspace(get_jpg[i])) {
				jpg_name[k] = get_jpg[i];
				k++;
			}
		}
		jpg_name[k] = '\0'; //���� �˷��� �Ѵ�. (�ƴϸ� �����Ⱚ ���)
		ZeroMemory(image_file_name, sizeof(image_file_name));
		ZeroMemory(query_jpg, sizeof(query_jpg));
		wsprintf(image_file_name, TEXT("%s%s"), get_cast, get_cast_type);
		sprintf(query_jpg, "%s%s.%s %s", "GET /", jpg_name, get_cast_type_for_socket, "HTTP/1.1\r\n\r\n");
		wcout << TEXT("query jpg : ") << endl;
		printf("%s\n", query_jpg);


		cases = 2; //�̹��� ������ cases = 2�� �ϰ� ��û

				   /* �����۸�ũ �κ� */
		image_hyperlink_maker(image_file_name); //�����۸�ũ �̸��� �̹��� �̸����� �ش�.
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

int input_valid_check(wstring get_str) /* �ּ� �Է� üũ */
{
	memset(&wow, 0x00, sizeof(wow));
	if (image_exist == 1) {
		image_exist = 0;
	}

	if (hwndStatic != NULL) {
		DestroyWindow(hwndStatic);
		temp_port2 = 0;
		hypertext = TEXT("");
	}


	char * domain_first = NULL;
	char * domain_middle = NULL;
	char * domain_third = NULL;
	char * for_dns = NULL; //dns�� ����
	char * temp_addr = NULL;
	wchar_t * temp_addr_view = NULL;
	char * pattern_str = NULL;
	memset(&dns_query_addr, 0x00, sizeof(dns_query_addr));

	string str;
	str.assign(get_str.begin(), get_str.end());

	std::transform(str.begin(), str.end(), str.begin(), tolower); // a�� �ҹ��ڷ� ����
	temp_addr = const_cast<char*>(str.c_str());
	temp_addr_view = const_cast<wchar_t*>(get_str.c_str());
	wcscpy(textbox_buffer, temp_addr_view);
	SetWindowText(Main_hWnd, temp_addr_view); //textbox ����
	SetWindowText(EdittextBox, temp_addr_view); //edittextbox ����



	char* pTemp = NULL;
	int iLen = WideCharToMultiByte(CP_ACP, 0, textbox_buffer, -1, pTemp, 0, NULL, NULL);
	pTemp = new char[iLen + 1];
	WideCharToMultiByte(CP_ACP, 0, textbox_buffer, -1, pTemp, iLen, NULL, NULL);
	//char chResult[MAX_PAHT] = "";
	//strcpy(chResult, pszTemp);
	//delete[] pTemp;

	printf("%d\n", pTemp[0]);

	
	if (pTemp[0] < 0) { //�ѱ��̸� �޸��忡�� �˻��Ѵ�.
		FILE * fpk = fopen("korean_page.txt", "r");
		if (fpk == NULL) {
			MessageBox(NULL, TEXT("���� �����ͺ��̽� ����!"), TEXT("����!"), MB_ICONINFORMATION);
			return -1;
		}

		char strTemp[255] = { 0 };
		while (!feof(fpk))
		{
			fgets(strTemp, sizeof(strTemp), fpk);
			if (!strncmp(pTemp, strTemp, strlen(pTemp))) { //�����ͺ��̽��� ������
				printf("%s", strTemp);
				strtok(strTemp, "/");
				hyperlink_maker(strtok(NULL, "/"));
				fclose(fpk);
				return 1;
			}
		}
		fclose(fpk);
		MessageBox(NULL, TEXT("�ּҸ� Ȯ���ϼ���!"), TEXT("����!"), MB_ICONINFORMATION);
		return -1; //����
	}
	

	//������� ��� ���ϰ˻� ���ϰ� ���⼭ �׳� �ٷ� ó��
	if ((str.at(0) == 'm') && (str.at(1) == '.')) {
		dns(temp_addr);
		clientsocket(dns_query_addr, 80);
		return 1;
	}

	if (!(str.at(0) == 'w') && !(str.at(0) == 'h')) //������ www �� http�� ���� ������
	{
		pattern_str = "(.*)\(com|co.kr|org|net|ac.kr|so|co|asia|go.kr|mil.kr|or.kr|kr|edu.mx)";
	}
	else {
		pattern_str = "(www|http://www|http://)\(.*)\(com|co.kr|org|net|ac.kr|so|co|asia|go.kr|mil.kr|or.kr|kr|edu.mx)";
	}
	regex pattern(pattern_str);

	smatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			cout << i << "��° : " << m[i] << endl;
		}
		string temps(m[1].str());
		string temps2(m[2].str()); //domain middle
		string temps3(m[3].str()); //domain third

		domain_first = const_cast<char*>(temps.c_str());
		domain_middle = const_cast<char*>(temps2.c_str());
		domain_third = const_cast<char*>(temps3.c_str());

		if (!strcmp(domain_first, "www")) {
			dns(temp_addr);
			clientsocket(dns_query_addr, 80);
		}
		else if (!strcmp(domain_first, "http://www")) {
			for_dns = new char[4 + strlen(domain_middle) + strlen(domain_third)];
			sprintf(for_dns, "%s%s%s", "www", domain_middle, domain_third);
			dns(for_dns);
			clientsocket(dns_query_addr, 80);
			delete[] for_dns;
		}
		else if (!strcmp(domain_first, "http://")) {
			for_dns = new char[5 + strlen(domain_middle) + strlen(domain_third)];
			sprintf(for_dns, "%s%s%s", "www.", domain_middle, domain_third);
			dns(for_dns);
			clientsocket(dns_query_addr, 80);
			delete[] for_dns;
		}
		else { //naver.com
			for_dns = new char[4 + strlen(domain_first) + strlen(domain_middle)];
			sprintf(for_dns, "%s%s%s", "www.", domain_first, domain_middle);
			dns(for_dns);
			clientsocket(dns_query_addr, 80);
			delete[] for_dns;
		}
	}
	else { //�������ݰ� �ּҸ� ��ū���� �и�
		if (strstr(temp_addr, ":1") || strstr(temp_addr, ":2") || strstr(temp_addr, ":3") || strstr(temp_addr, ":4") || strstr(temp_addr, ":5") || strstr(temp_addr, ":6") ||
			strstr(temp_addr, ":7") || strstr(temp_addr, ":8") || strstr(temp_addr, ":9"))
		{
			if (strstr(temp_addr, "http://")) { //�տ� http�� ���� ���
				strtok(temp_addr, "/");
				//printf("ù��° ��ū : temp_addr : %s\n", strtok(NULL,"/"));
				strcpy(temp_addr2, strtok(NULL, "/"));
				//printf("�ι�° ��ū : temp_addr : %s\n", strtok(temp_addr2, ":"));
				strtok(temp_addr2, ":");
				char   *ptr = NULL;
				char * temp_PORT = strtok(NULL, ":");
				//int temp_port = atoi(temp_PORT); //atoi ����
				int temp_port = strtol(temp_PORT, &ptr, 10);
				temp_port2 = temp_port;
				cases = 1;
				clientsocket(temp_addr2, temp_port2);
			}
			else {
				strtok(temp_addr, ":");
				strcpy(temp_addr2, temp_addr);
				char * temp_PORT = strtok(NULL, ":");
				char   *ptr = NULL;
				//int temp_port = atoi(temp_PORT);
				int temp_port = strtol(temp_PORT,&ptr,10);
				temp_port2 = temp_port;
				cases = 1;
				clientsocket(temp_addr2, temp_port2);
			}
		}
		else {
			MessageBox(NULL, TEXT("�ּҸ� Ȯ���ϼ���!"), TEXT("����!"), MB_ICONINFORMATION);
			return -1; //����
		}


	}
	return 1;
}

wstring replaceAll(const wstring &str, const wstring &pattern, const wstring &replace)
{
	wstring result = str;
	wstring::size_type pos = 0;
	wstring::size_type offset = 0;
	while ((pos = result.find(pattern, offset)) != wstring::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}
	return result;
}
/*
6. ���ڿ� ����
erase()�� ���ڿ��� �Ϻκ��� �����ϰ� clear() �� ������ �����Ѵ�.

string a("I love C++");
a.erase(0, 7); // a�� ó������ 7���� ���� ����. a = "C++"�� ����
a.clear(); // a = ""

*/


wstring eraseTag(const wstring &str, const wstring &pattern) { //�±� ���� char ������ ����
	wstring result = str;
	wstring::size_type start_pos = 0;
	wstring::size_type end_pos = 0;
	wstring::size_type offset = 0;

	
	while ((start_pos = result.find(pattern, offset)) != wstring::npos)
	{
		if (start_pos > 0) {
			printf("ã�Ҵ�!!!");
			printf("��ġ : %d\n", start_pos);
			while ((end_pos = result.find(TEXT(">"), start_pos)) != wstring::npos) {
				printf("��ã�Ҵ�!!!");
				printf("��ġ : %d\n", end_pos);
				if (end_pos > 0) {
					result.erase(start_pos, end_pos);
					break; //����� Ż��
				}
			}
			offset = end_pos;
		}
		start_pos = 0;
		end_pos = 0;
	}

	//find�� < ��ġ Ž���Ͽ� ��ġ ��ȯ
	//find�� > ��ġ Ž���Ͽ� ��ġ ��ȯ
	//replace�� ��� ����
	return result;
}


void parser(wstring before_parser)
{
	totalresult.clear(); //Ŭ��� ó���� ����� �Ѵ�.
						 //�ļ����� ���� �����̶� ĳ���� ���� ��� �� ������
		
	wstring result;
	result = replaceAll(before_parser, TEXT("\r"), TEXT(""));
	result = replaceAll(result, TEXT("\n"), TEXT(""));
	result = replaceAll(result, TEXT("\r\n"), TEXT(""));
	//result = eraseTag(result, TEXT("<")); //�±� ����
	
	wstring headerresult = Search_header(result);
	wstring bodyresult = Search_body(result);
	wstring headcontent = TEXT("HEAD TEXT\n\n");
	wstring bodycontent = TEXT("\n\nBODY TEXT\n\n");

	totalresult.append(headcontent).append(headerresult).append(bodycontent).append(bodyresult);
	
	wow = const_cast<wchar_t*>(totalresult.c_str());

	if (cases == 1) { //�ϴ� CGI �϶��� �̹��� ���� ��û
		Search_jpg_image(result); //gif �� ���̹��� �ֱ� ������ ������ ���� ����
	}
	cases = 0;
}
