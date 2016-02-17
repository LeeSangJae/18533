#pragma once
#include "header.h"
#include "hyperlink.h"

char *get_temp;
char *get_temp_body;
int get_size_header, get_size_body;
char *image_file_name;
char *query_jpg_real;

void Search_header(string get_str)
{
	char * get_cast;
	regex pattern("(<head>|<HEAD>)\(.*)\(</head>|</HEAD>)");
	string str = get_str;
	//string temps;
	smatch m;
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			cout << i << "��° : " << m[i] << endl;
		}
		string temps(m[2].str());
		get_cast = const_cast<char*>(temps.c_str());
		get_size_header = strlen(get_cast);
		get_temp = new char[get_size_header];
		sprintf(get_temp, "%s", get_cast);
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

void Search_body(string get_str)
{
	char * get_cast;
	regex pattern("(<body>|<BODY>)\(.*)\(</body>|</BODY>)");
	string str = get_str;
	//string temps;
	smatch m;
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			cout << i << "��° : " << m[i] << endl;
		}
		string temps(m[2].str());
		get_cast = const_cast<char*>(temps.c_str());
		get_size_body = strlen(get_cast);
		get_temp_body = new char[get_size_body];
		sprintf(get_temp_body, "%s", get_cast);
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}


void Search_jpg_image(string get_str)
{
	char * get_cast;
	char * get_cast_type; //�̹��� Ÿ��
	char query_jpg[100];
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)\(\"></br>)"); //�ϴ� ��δ� �ļ�
	regex pattern("\"\(.*)\(bmp)"); //ȿ�ٴԲ����� bmp�� �޾ƿ���
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)"); //�������� �ļ� (�̹��� 1���� ��)

	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)\>"); //�̹��� ������ ã��
	string str = get_str;
	//string temps;
	smatch m;
	char get_jpg[20];
	char jpg_name[20];
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
		image_file_name = new char[strlen(get_cast) + strlen(get_cast_type) + 1];
		sprintf(image_file_name, "%s%s", get_cast, get_cast_type);
		sprintf(query_jpg, "%s%s.%s %s", "GET /", jpg_name,get_cast_type, "HTTP/1.1\r\n\r\n");
		printf("query_jpg: %s \n", query_jpg);

		//query_jpg_real = (char*)malloc(strlen(query_jpg) + 1);
		query_jpg_real = new char[strlen(query_jpg) + 1];
		strcpy(query_jpg_real, query_jpg);

		cases = 2; //�̹��� ������ cases = 2�� �ϰ� ��û
		
		/* �����۸�ũ �κ� */
		image_hyperlink_maker(image_file_name); //�����۸�ũ �̸��� �̹��� �̸����� �ش�.
		//clientsocket(textbox_buffer, PORT_NUM); //�� �κп� ��Ʈ��ȣ�� �߸� �ְ� �־���
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

void input_valid_check(string get_str) /* �ּ� �Է� üũ */
{
	char * domain_first;
	char * domain_middle;
	char * domain_third;
	char * for_dns; //dns�� ����
	char * temp_addr;
	regex pattern("(www|http://www|http://)\(.*)\(com|co.kr|org|net)");
	string str = get_str;
	temp_addr = const_cast<char*>(get_str.c_str());
	strcpy(textbox_buffer, temp_addr);
	SetWindowText(Main_hWnd, temp_addr); //textbox ����
	SetWindowText(EdittextBox, temp_addr); //edittextbox ����

	smatch m;
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
			for_dns = new char[4+strlen(domain_middle) + strlen(domain_third)];
			sprintf(for_dns, "%s%s%s", "www",domain_middle, domain_third);
			dns(for_dns);
			clientsocket(dns_query_addr, 80);
			delete[] for_dns;
		}
		else if (!strcmp(domain_first, "http://")) {
			for_dns = new char[5+strlen(domain_middle) + strlen(domain_third)];
			sprintf(for_dns, "%s%s%s", "www.", domain_middle, domain_third);
			dns(for_dns);
			clientsocket(dns_query_addr, 80);
			delete[] for_dns;
		}
	}
	else {
		cases = 1;
		clientsocket(temp_addr, PORT_NUM);
	}
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
	//�ļ����� ���� �����̶� ĳ���� ���� ��� �� ������
	string result = replaceAll(before_parser, "\r", "");
	result = replaceAll(result, "\n", "");
	result = replaceAll(result, "\r\n", "");
	Search_header(result);
	Search_body(result);
	totalresult = new char[get_size_header + get_size_body + 100];
	sprintf(totalresult, "[ HEAD CONTENT ]\n%s\n\n [BODY CONTENT ]\n%s", get_temp, get_temp_body);
	if (cases == 1) { //�ϴ� CGI �϶��� �̹��� ���� ��û
		Search_jpg_image(result); //gif �� ���̹��� �ֱ� ������ ������ ���� ����
	}

	cases = 0;
	//delete []get_temp;
	//delete []get_temp_body;
	//delete []totalresult;
}




/*
int boost_regex(char * str) {
std::string sre; //rull
boost::regex re;

sre = ("(<head>)\.*\(</head>)");

try
{
// Set up the regular expression for case-insensitivity
re.assign(sre, boost::regex_constants::icase);
}
catch (boost::regex_error& e)
{
cout << sre << " is not a valid regular expression: \""
<< e.what() << "\"" << endl;
return -1;
}
if (boost::regex_search(str, re)) //not regex_match (�� ��ġ�ؾ� ��)
{
cout << re << " matches " << str << endl;
return 1;
}

}
*/