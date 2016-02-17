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
			cout << i << "번째 : " << m[i] << endl;
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
			cout << i << "번째 : " << m[i] << endl;
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
	char * get_cast_type; //이미지 타입
	char query_jpg[100];
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)\(\"></br>)"); //일단 재민님 파서
	regex pattern("\"\(.*)\(bmp)"); //효근님꺼에서 bmp만 받아오기
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)"); //오리지날 파서 (이미지 1개일 때)

	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)\>"); //이미지 파일을 찾음
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
			cout << i << "번째 : " << m[i] << endl;
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
		jpg_name[k] = '\0'; //끝을 알려야 한다. (아니면 쓰레기값 출력)
		image_file_name = new char[strlen(get_cast) + strlen(get_cast_type) + 1];
		sprintf(image_file_name, "%s%s", get_cast, get_cast_type);
		sprintf(query_jpg, "%s%s.%s %s", "GET /", jpg_name,get_cast_type, "HTTP/1.1\r\n\r\n");
		printf("query_jpg: %s \n", query_jpg);

		//query_jpg_real = (char*)malloc(strlen(query_jpg) + 1);
		query_jpg_real = new char[strlen(query_jpg) + 1];
		strcpy(query_jpg_real, query_jpg);

		cases = 2; //이미지 있으면 cases = 2로 하고 요청
		
		/* 하이퍼링크 부분 */
		image_hyperlink_maker(image_file_name); //하이퍼링크 이름은 이미지 이름으로 준다.
		//clientsocket(textbox_buffer, PORT_NUM); //이 부분에 포트번호를 잘못 주고 있었음
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

void input_valid_check(string get_str) /* 주소 입력 체크 */
{
	char * domain_first;
	char * domain_middle;
	char * domain_third;
	char * for_dns; //dns를 위해
	char * temp_addr;
	regex pattern("(www|http://www|http://)\(.*)\(com|co.kr|org|net)");
	string str = get_str;
	temp_addr = const_cast<char*>(get_str.c_str());
	strcpy(textbox_buffer, temp_addr);
	SetWindowText(Main_hWnd, temp_addr); //textbox 갱신
	SetWindowText(EdittextBox, temp_addr); //edittextbox 갱신

	smatch m;
	if (regex_search(str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			cout << i << "번째 : " << m[i] << endl;
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
	//파서에서 먼저 개행이랑 캐리지 리턴 모두 다 없애줌
	string result = replaceAll(before_parser, "\r", "");
	result = replaceAll(result, "\n", "");
	result = replaceAll(result, "\r\n", "");
	Search_header(result);
	Search_body(result);
	totalresult = new char[get_size_header + get_size_body + 100];
	sprintf(totalresult, "[ HEAD CONTENT ]\n%s\n\n [BODY CONTENT ]\n%s", get_temp, get_temp_body);
	if (cases == 1) { //일단 CGI 일때만 이미지 파일 요청
		Search_jpg_image(result); //gif 가 네이버에 있기 때문에 에러가 나는 것임
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
if (boost::regex_search(str, re)) //not regex_match (꼭 일치해야 함)
{
cout << re << " matches " << str << endl;
return 1;
}

}
*/