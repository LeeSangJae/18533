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
			//cout << i << "번째 : " << m[i] << endl;
		}
		string temps(m[2].str());
		return temps;
	}
	else {
		return "헤더가 없음";
		cout << "헤더가 일치하지 않음" << endl;
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
			//cout << i << "번째 : " << m[i] << endl;
		}
		string temps(m[2].str());
		return temps;
	}
	else {
		return "바디가 없음";
		cout << "바디가 일치하지 않음" << endl;
	}
}


void Search_jpg_image(string get_str)
{
	char * get_cast = NULL;
	char * get_cast_type = NULL; //이미지 타입

	char * pattern_str = NULL;
	if (temp_port2 == 8090)//재민님
		pattern_str = "\"\(.*)\(jpg|bmp|gif|jpeg)\(\"></br>)";
	else if (temp_port2 = 8677)//효근님꺼에서 bmp만 받아오기;
		pattern_str = "\"\(.*)\(bmp)";
	else if (temp_port2 = 8979) //경미님
		pattern_str = "\"\(.*)\(jpg|bmp|gif|jpeg)\">";

	if (pattern_str == NULL)
		return;

	cout << "찾는 문자열 : " << get_str << endl;

	regex pattern(pattern_str); // 재민님 패턴
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)"); //오리지날 파서 (이미지 1개일 때)
	//regex pattern("\"\(.*)\(jpg|bmp|gif|jpeg)\>"); //이미지 파일을 찾음
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
		//image_file_name = new char[strlen(get_cast) + strlen(get_cast_type) + 1];
		ZeroMemory(image_file_name, sizeof(image_file_name));
		ZeroMemory(query_jpg, sizeof(query_jpg));
		sprintf(image_file_name, "%s%s", get_cast, get_cast_type);
		sprintf(query_jpg, "%s%s.%s %s", "GET /", jpg_name, get_cast_type, "HTTP/1.1\r\n\r\n");
		printf("query_jpg: %s \n", query_jpg);

		//query_jpg_real = (char*)malloc(strlen(query_jpg) * sizeof(char)); //두번 메모리 할당하고 있었음
		//query_jpg_real = new char[strlen(query_jpg)];
		//strcpy(query_jpg_real, query_jpg);

		cases = 2; //이미지 있으면 cases = 2로 하고 요청

		/* 하이퍼링크 부분 */
		image_hyperlink_maker(image_file_name); //하이퍼링크 이름은 이미지 이름으로 준다.
		//clientsocket(textbox_buffer, PORT_NUM); //이 부분에 포트번호를 잘못 주고 있었음
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

int input_valid_check(string get_str) /* 주소 입력 체크 */
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
	char * for_dns = NULL; //dns를 위해
	char * temp_addr = NULL;
	char * pattern_str = NULL;
	memset(&dns_query_addr, 0x00, sizeof(dns_query_addr));


	string str = get_str;
	std::transform(str.begin(), str.end(), str.begin(), tolower); // a를 소문자로 변경
	temp_addr = const_cast<char*>(str.c_str());
	strcpy(textbox_buffer, temp_addr);
	SetWindowText(Main_hWnd, temp_addr); //textbox 갱신
	SetWindowText(EdittextBox, temp_addr); //edittextbox 갱신

	//모바일인 경우 패턴검색 안하고 여기서 그냥 바로 처리
	if ((textbox_buffer[0]) == 'm' && (textbox_buffer[1] == '.')) {
		dns(temp_addr);
		clientsocket(dns_query_addr, 80);
		return 1;
	}

	if (!(textbox_buffer[0] == 'w') && !(textbox_buffer[0] == 'h')) //시작이 www로 되지 않으면
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
	else { //프로토콜과 주소를 토큰으로 분리
		if (strstr(temp_addr, ":1") || strstr(temp_addr, ":2") || strstr(temp_addr, ":3") || strstr(temp_addr, ":4") || strstr(temp_addr, ":5") || strstr(temp_addr, ":6") ||
			strstr(temp_addr, ":7") || strstr(temp_addr, ":8") || strstr(temp_addr, ":9"))
		{
			if (strstr(temp_addr, "http://")) { //앞에 http가 붙은 경우
				strtok(temp_addr, "/");
				//printf("첫번째 토큰 : temp_addr : %s\n", strtok(NULL,"/"));
				strcpy(temp_addr2, strtok(NULL, "/"));
				printf("두번째 토큰 : temp_addr : %s\n", strtok(temp_addr2, ":"));
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
			MessageBox(NULL, "주소를 확인하세요!", "에러!", MB_ICONINFORMATION);
			return -1; //에러
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
	totalresult.clear(); //클리어를 처음에 해줘야 한다.
	//파서에서 먼저 개행이랑 캐리지 리턴 모두 다 없애줌
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
	if (cases == 1) { //일단 CGI 일때만 이미지 파일 요청
		Search_jpg_image(result); //gif 가 네이버에 있기 때문에 에러가 나는 것임
	}
	cases = 0;
	//delete []get_temp;
	//delete []get_temp_body;
	//delete []totalresult;
}