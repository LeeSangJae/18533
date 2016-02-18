#pragma once
#include "header.h"
#include "hyperlink.h"

char image_file_name[100] = { 0 };
int image_exist = 0;
char query_jpg[100] = { 0 };

string Search_header(string get_str)
{
	regex pattern("(<head>|<HEAD>)\(.*)\(</head>|</HEAD>)");
	string str = get_str;
	smatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "��° : " << m[i] << endl;
		}
		string temps(m[2].str());
		return temps;
	}
	else {
		return "����� ����";
		cout << "����� ��ġ���� ����" << endl;
	}
}

string Search_body(string get_str)
{
	regex pattern("(<body>|<BODY>|<body|<BODY|<script)\(.*)\(</body>|</BODY>|/body>|/BODY>|</script>)");
	string str = get_str;
	//string temps;
	smatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "��° : " << m[i] << endl;
		}
		string temps(m[2].str());
		return temps;
	}
	else {
		return "�ٵ� ����";
		cout << "�ٵ� ��ġ���� ����" << endl;
	}
}


void Search_jpg_image(string get_str)
{
	char * get_cast = NULL;
	char * get_cast_type = NULL; //�̹��� Ÿ��

	char * pattern_str = NULL;
	if (temp_port2 == 8090)//��δ�
		pattern_str = "\"\(.*)\(jpg|bmp|gif|jpeg)\(\"></br>)";
	else if (temp_port2 = 8677)//ȿ�ٴԲ����� bmp�� �޾ƿ���;
		pattern_str = "\"\(.*)\(bmp)";
	else if (temp_port2 = 8979) //��̴�
		pattern_str = "\"\(.*)\(jpg|bmp|gif|jpeg)\">";

	if (pattern_str == NULL)
		return;

	cout << "ã�� ���ڿ� : " << get_str << endl;

	regex pattern(pattern_str); // ��δ� ����
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)"); //�������� �ļ� (�̹��� 1���� ��)
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)\>"); //�̹��� ������ ã��
	string str = get_str;
	//string temps;
	smatch m;
	ZeroMemory(&m, sizeof(m));
	char get_jpg[20] = { 0 };
	char jpg_name[20] = { 0 };
	int k = 0;
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			cout << i << "��° : " << m[i] << endl;
		}
		string temps(m[1].str());
		string tempstype(m[2].str());
		get_cast = const_cast<char*>(temps.c_str());
		get_cast_type = const_cast<char*>(tempstype.c_str());
		sprintf(get_jpg, "%s", get_cast);
		for (int i = 0; i < strlen(get_jpg); i++)
		{
			if (!IsCharAlphaNumeric(get_jpg[i])) {
				get_jpg[i] = ' ';
			}
		}
		for (int i = 0; i < strlen(get_jpg); i++)
		{
			if (!isspace(get_jpg[i])) {
				jpg_name[k] = get_jpg[i];
				k++;
			}
		}
		jpg_name[k] = '\0'; //���� �˷��� �Ѵ�. (�ƴϸ� �����Ⱚ ���)
		//image_file_name = new char[strlen(get_cast) + strlen(get_cast_type) + 1];
		ZeroMemory(image_file_name, sizeof(image_file_name));
		ZeroMemory(query_jpg, sizeof(query_jpg));
		sprintf(image_file_name, "%s%s", get_cast, get_cast_type);
		sprintf(query_jpg, "%s%s.%s %s", "GET /", jpg_name, get_cast_type, "HTTP/1.1\r\n\r\n");
		printf("query_jpg: %s \n", query_jpg);

		//query_jpg_real = (char*)malloc(strlen(query_jpg) * sizeof(char)); //�ι� �޸� �Ҵ��ϰ� �־���
		//query_jpg_real = new char[strlen(query_jpg)];
		//strcpy(query_jpg_real, query_jpg);

		cases = 2; //�̹��� ������ cases = 2�� �ϰ� ��û

		/* �����۸�ũ �κ� */
		image_hyperlink_maker(image_file_name); //�����۸�ũ �̸��� �̹��� �̸����� �ش�.
		//clientsocket(textbox_buffer, PORT_NUM); //�� �κп� ��Ʈ��ȣ�� �߸� �ְ� �־���
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

int input_valid_check(string get_str) /* �ּ� �Է� üũ */
{
	memset(&wow, 0x00, sizeof(wow));
	if (image_exist == 1) {
		image_exist = 0;
	}

	if (hwndStatic != NULL) {
		DestroyWindow(hwndStatic);
		temp_port2 = 0;
		hypertext = "";
	}


	char * domain_first = NULL;
	char * domain_middle = NULL;
	char * domain_third = NULL;
	char * for_dns = NULL; //dns�� ����
	char * temp_addr = NULL;
	char * pattern_str = NULL;
	memset(&dns_query_addr, 0x00, sizeof(dns_query_addr));


	string str = get_str;
	std::transform(str.begin(), str.end(), str.begin(), tolower); // a�� �ҹ��ڷ� ����
	temp_addr = const_cast<char*>(str.c_str());
	strcpy(textbox_buffer, temp_addr);
	SetWindowText(Main_hWnd, temp_addr); //textbox ����
	SetWindowText(EdittextBox, temp_addr); //edittextbox ����

	//������� ��� ���ϰ˻� ���ϰ� ���⼭ �׳� �ٷ� ó��
	if ((textbox_buffer[0]) == 'm' && (textbox_buffer[1] == '.')) {
		dns(temp_addr);
		clientsocket(dns_query_addr, 80);
		return 1;
	}

	if (!(textbox_buffer[0] == 'w') && !(textbox_buffer[0] == 'h')) //������ www�� ���� ������
	{
		pattern_str = "(.*)\(com|co.kr|org|net|ac.kr)";
	}
	else {
		pattern_str = "(www|http://www|http://)\(.*)\(com|co.kr|org|net|ac.kr)";
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
			printf("%s\n", for_dns);
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
				printf("�ι�° ��ū : temp_addr : %s\n", strtok(temp_addr2, ":"));
				char * temp_PORT = strtok(NULL, ":");
				int temp_port = atoi(temp_PORT);
				temp_port2 = temp_port;
				cases = 1;
				clientsocket(temp_addr2, temp_port2);
			}
			else {
				strtok(temp_addr, ":");
				strcpy(temp_addr2, temp_addr);
				char * temp_PORT = strtok(NULL, ":");
				int temp_port = atoi(temp_PORT);
				temp_port2 = temp_port;
				cases = 1;
				clientsocket(temp_addr2, temp_port2);
			}
		}
		else {
			MessageBox(NULL, "�ּҸ� Ȯ���ϼ���!", "����!", MB_ICONINFORMATION);
			return -1; //����
		}


	}
	return 1;
}

string replaceAll(const string &str, const string &pattern, const string &replace)
{
	string result = str;
	string::size_type pos = 0;
	string::size_type offset = 0;
	while ((pos = result.find(pattern, offset)) != string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}
	return result;
}


void parser(string before_parser)
{
	totalresult.clear(); //Ŭ��� ó���� ����� �Ѵ�.
	//�ļ����� ���� �����̶� ĳ���� ���� ��� �� ������
	string result;
	result = replaceAll(before_parser, "\r", "");
	result = replaceAll(result, "\n", "");
	result = replaceAll(result, "\r\n", "");
	string headerresult = Search_header(result);
	string bodyresult = Search_body(result);
	string headcontent = "[ HEAD CONTENT ]\n";
	string bodycontent = "\n\n [BODY CONTENT ]\n";

	totalresult.append(headcontent).append(headerresult).append(bodycontent).append(bodyresult);
	wow = const_cast<char*>(totalresult.c_str());
	if (cases == 1) { //�ϴ� CGI �϶��� �̹��� ���� ��û
		Search_jpg_image(result); //gif �� ���̹��� �ֱ� ������ ������ ���� ����
	}
	cases = 0;
	//delete []get_temp;
	//delete []get_temp_body;
	//delete []totalresult;
}