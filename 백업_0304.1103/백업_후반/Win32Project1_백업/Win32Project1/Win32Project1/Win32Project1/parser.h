#pragma once
#include "header.h"
#include "hyperlink.h"

wchar_t image_file_name[100] = { 0 };
int image_exist = 0;
char query_jpg[100] = { 0 };

wstring Search_header(wstring get_str)
{
	wregex pattern(TEXT("(<head>|<HEAD>|<head|<HEAD)\(.*)\(</head>|</HEAD>|/head>|/HEAD>)"));
	
	wsmatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(get_str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "¹øÂ° : " << m[i] << endl;
		}
		return m[2];
	}
	else {
		return TEXT("Çì´õ°¡ ¾øÀ½");
		wcout << TEXT("Çì´õ°¡ ÀÏÄ¡ÇÏÁö ¾ÊÀ½") << endl;
	}
}

wstring Search_body(wstring get_str)
{
	wregex pattern(TEXT("(<body>|<BODY>|<BODY|<body)\(.*)\(</body>|</BODY>|/body>|/BODY>)"));
	wsmatch m;
	ZeroMemory(&m, sizeof(m));
	if (regex_search(get_str, m, pattern))
	{
		for (size_t i = 0; i < m.size(); i++)
		{
			//cout << i << "¹øÂ° : " << m[i] << endl;
		}
		return m[2];
	}
	else {
		return TEXT("¹Ùµð°¡ ¾øÀ½");
		wcout << TEXT("¹Ùµð°¡ ÀÏÄ¡ÇÏÁö ¾ÊÀ½") << endl;
	}
}


void Search_jpg_image(wstring get_str)
{
	wchar_t * get_cast = NULL;
	wchar_t * get_cast_type = NULL; //ÀÌ¹ÌÁö Å¸ÀÔ
	char * get_cast_type_for_socket = NULL;

	wchar_t * pattern_str = NULL;
	if (temp_port2 == 8090)//Àç¹Î´Ô
		pattern_str = TEXT("\"\(.*)\(jpg|bmp|gif|jpeg)\(\"></br>)");
	else if (temp_port2 = 8677)//È¿±Ù´Ô²¨¿¡¼­ bmp¸¸ ¹Þ¾Æ¿À±â;
		pattern_str = TEXT("\"\(.*)\(bmp)");
	else if (temp_port2 = 8979) //°æ¹Ì´Ô
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
			//cout << i << "¹øÂ° : " << m[i] << endl;
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
		jpg_name[k] = '\0'; //³¡À» ¾Ë·Á¾ß ÇÑ´Ù. (¾Æ´Ï¸é ¾²·¹±â°ª Ãâ·Â)
		ZeroMemory(image_file_name, sizeof(image_file_name));
		ZeroMemory(query_jpg, sizeof(query_jpg));
		wsprintf(image_file_name, TEXT("%s%s"), get_cast, get_cast_type);
		sprintf(query_jpg, "%s%s.%s %s", "GET /", jpg_name, get_cast_type_for_socket, "HTTP/1.1\r\n\r\n");
		wcout << TEXT("query jpg : ") << endl;
		printf("%s\n", query_jpg);


		cases = 2; //ÀÌ¹ÌÁö ÀÖÀ¸¸é cases = 2·Î ÇÏ°í ¿äÃ»

				   /* ÇÏÀÌÆÛ¸µÅ© ºÎºÐ */
		image_hyperlink_maker(image_file_name); //ÇÏÀÌÆÛ¸µÅ© ÀÌ¸§Àº ÀÌ¹ÌÁö ÀÌ¸§À¸·Î ÁØ´Ù.
	}
	else {
		cout << "NOT MATCH" << endl;
	}
}

int input_valid_check(wstring get_str) /* ÁÖ¼Ò ÀÔ·Â Ã¼Å© */
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

	if (visit_log_delete_button != NULL)
	{
		DestroyWindow(visit_log_delete_button);
	}


	char * domain_first = NULL;
	char * domain_middle = NULL;
	char * domain_third = NULL;
	char * for_dns = NULL; //dns¸¦ À§ÇØ
	char * temp_addr = NULL;
	wchar_t * temp_addr_view = NULL;
	char * pattern_str = NULL;
	memset(&dns_query_addr, 0x00, sizeof(dns_query_addr));

	string str;
	str.assign(get_str.begin(), get_str.end());

	std::transform(str.begin(), str.end(), str.begin(), tolower); // a¸¦ ¼Ò¹®ÀÚ·Î º¯°æ
	temp_addr = const_cast<char*>(str.c_str());
	temp_addr_view = const_cast<wchar_t*>(get_str.c_str());
	wcscpy(textbox_buffer, temp_addr_view);
	SetWindowText(Main_hWnd, temp_addr_view); //textbox °»½Å
	SetWindowText(EdittextBox, temp_addr_view); //edittextbox °»½Å



	char* pTemp = NULL;
	int iLen = WideCharToMultiByte(CP_ACP, 0, textbox_buffer, -1, pTemp, 0, NULL, NULL);
	pTemp = new char[iLen + 1];
	WideCharToMultiByte(CP_ACP, 0, textbox_buffer, -1, pTemp, iLen, NULL, NULL);
	//char chResult[MAX_PAHT] = "";
	//strcpy(chResult, pszTemp);
	//delete[] pTemp;

	printf("%d\n", pTemp[0]);

	
	if (pTemp[0] < 0) { //ÇÑ±ÛÀÌ¸é ¸Þ¸ðÀå¿¡¼­ °Ë»çÇÑ´Ù.
		FILE * fpk = fopen("korean_page.txt", "r");
		if (fpk == NULL) {
			MessageBox(NULL, TEXT("¼­¹ö µ¥ÀÌÅÍº£ÀÌ½º ¿¡·¯!"), TEXT("¿¡·¯!"), MB_ICONINFORMATION);
			return -1;
		}

		char strTemp[255] = { 0 };
		while (!feof(fpk))
		{
			fgets(strTemp, sizeof(strTemp), fpk);
			if (!strncmp(pTemp, strTemp, strlen(pTemp))) { //µ¥ÀÌÅÍº£ÀÌ½º¿¡ ÀÖÀ¸¸é
				printf("%s", strTemp);
				strtok(strTemp, "/");
				hyperlink_maker(strtok(NULL, "/"));
				fclose(fpk);
				return 1;
			}
		}
		fclose(fpk);
		MessageBox(NULL, TEXT("ÁÖ¼Ò¸¦ È®ÀÎÇÏ¼¼¿ä!"), TEXT("¿¡·¯!"), MB_ICONINFORMATION);
		return -1; //¿¡·¯
	}
	

	//¸ð¹ÙÀÏÀÎ °æ¿ì ÆÐÅÏ°Ë»ö ¾ÈÇÏ°í ¿©±â¼­ ±×³É ¹Ù·Î Ã³¸®
	if ((str.at(0) == 'm') && (str.at(1) == '.')) {
		dns(temp_addr);
		clientsocket(dns_query_addr, 80);
		return 1;
	}

	if (!(str.at(0) == 'w') && !(str.at(0) == 'h')) //½ÃÀÛÀÌ www ¶û http·Î µÇÁö ¾ÊÀ¸¸é
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
			cout << i << "¹øÂ° : " << m[i] << endl;
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
	else { //ÇÁ·ÎÅäÄÝ°ú ÁÖ¼Ò¸¦ ÅäÅ«À¸·Î ºÐ¸®
		if (strstr(temp_addr, ":1") || strstr(temp_addr, ":2") || strstr(temp_addr, ":3") || strstr(temp_addr, ":4") || strstr(temp_addr, ":5") || strstr(temp_addr, ":6") ||
			strstr(temp_addr, ":7") || strstr(temp_addr, ":8") || strstr(temp_addr, ":9"))
		{
			if (strstr(temp_addr, "http://")) { //¾Õ¿¡ http°¡ ºÙÀº °æ¿ì
				strtok(temp_addr, "/");
				//printf("Ã¹¹øÂ° ÅäÅ« : temp_addr : %s\n", strtok(NULL,"/"));
				strcpy(temp_addr2, strtok(NULL, "/"));
				//printf("µÎ¹øÂ° ÅäÅ« : temp_addr : %s\n", strtok(temp_addr2, ":"));
				strtok(temp_addr2, ":");
				char   *ptr = NULL;
				char * temp_PORT = strtok(NULL, ":");
				//int temp_port = atoi(temp_PORT); //atoi ¿¡·¯
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
			MessageBox(NULL, TEXT("ÁÖ¼Ò¸¦ È®ÀÎÇÏ¼¼¿ä!"), TEXT("¿¡·¯!"), MB_ICONINFORMATION);
			return -1; //¿¡·¯
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
6. ¹®ÀÚ¿­ »èÁ¦
erase()´Â ¹®ÀÚ¿­ÀÇ ÀÏºÎºÐÀ» »èÁ¦ÇÏ°í clear() ´Â ¿ÏÀüÈ÷ »èÁ¦ÇÑ´Ù.

string a("I love C++");
a.erase(0, 7); // aÀÇ Ã³À½ºÎÅÍ 7°³ÀÇ ¹®ÀÚ »èÁ¦. a = "C++"·Î º¯°æ
a.clear(); // a = ""

*/

/*
wstring eraseTag(const wstring &str, const wstring &pattern) { //ÅÂ±× Á¦°Å char ´ÜÀ§·Î ¼öÇà
	wstring result = str;
	wstring::size_type start_pos = 0;
	wstring::size_type end_pos = 0;
	wstring::size_type offset = 0;

	
	while ((start_pos = result.find(pattern, offset)) != wstring::npos)
	{
		if (start_pos > 0) {
			while ((end_pos = result.find(TEXT(">"), start_pos)) != wstring::npos) {
				if (end_pos > 0) {
					result.erase(start_pos, end_pos);
					break; //Áö¿ì°í Å»Ãâ
				}
			}
			offset = end_pos+1;
		}
		start_pos = 0;
		end_pos = 0;
	}
	return result;
}
*/

void parser(wstring before_parser)
{
	totalresult.clear(); //Å¬¸®¾î¸¦ Ã³À½¿¡ ÇØÁà¾ß ÇÑ´Ù.
						 //ÆÄ¼­¿¡¼­ ¸ÕÀú °³ÇàÀÌ¶û Ä³¸®Áö ¸®ÅÏ ¸ðµÎ ´Ù ¾ø¾ÖÁÜ
		
	wstring result;
	result = replaceAll(before_parser, TEXT("\r"), TEXT(""));
	result = replaceAll(result, TEXT("\n"), TEXT(""));
	result = replaceAll(result, TEXT("\r\n"), TEXT(""));
	result = replaceAll(result, TEXT("|"), TEXT(""));
	wregex style_html(TEXT("(<STYLE>|<style>|<STYLE|<style)\(.*)\(</STYLE>|</style>|/STYLE>|/style>)")); //styleÁ¦°Å
	//wregex javascript(TEXT("(<script|<SCRIPT|<script>|<SCRIPT>)\(.*)\(</script>|</SCRIPT>|/script>|/SCRIPT>)")); //½ºÅ©¸³Æ®ºÎÅÍ Á¦°Å
	wregex rgx(TEXT("<[\-\_\~\`\!\@\#\$\%\^\&+\*\(\)\{\}\=\:\;\"\'\?\&\.\,°¡-ÆR0-9a-zA-Z /]*>")); //¼ýÀÚ, ¾ËÆÄºª, °ø¹é À» Æ÷ÇÔÇÏ°í ÀÖ´Â ÅÂ±× Á¦°Å
	wregex spaceremove(TEXT("(  |<|>)")); //°ø¹é µÎ¹ø ÀÖ´ÂºÎºÐ Á¦°Å
	wregex entermaker(TEXT("(@@+)"));
	wregex golremove(TEXT("(@)"));
	
	
	wstring headerresult = Search_header(result);
	headerresult = regex_replace(headerresult, style_html, TEXT("")); //½ºÅ¸ÀÏ ¸ÕÀú Á¦°Å
	//headerresult = regex_replace(headerresult, javascript, TEXT(""));
	headerresult = regex_replace(headerresult, rgx, TEXT("@")); //@·Î º¯È¯ÈÄ @°¡ ÇÑ °³ÀÌ¸é ¾ø¾ÖÁÖ°í @°¡ µÎ °³ÀÌ¸é ¿£ÅÍ³Ö±â
	headerresult = regex_replace(headerresult, spaceremove, TEXT(""));
	headerresult = regex_replace(headerresult, entermaker, TEXT("\n"));
	headerresult = regex_replace(headerresult, golremove, TEXT("\n"));

	wstring bodyresult = Search_body(result);
	bodyresult = regex_replace(bodyresult, style_html, TEXT(""));
	//bodyresult = regex_replace(bodyresult, javascript, TEXT(""));
	bodyresult = regex_replace(bodyresult, rgx, TEXT("@"));
	bodyresult = regex_replace(bodyresult, spaceremove, TEXT(""));
	bodyresult = regex_replace(bodyresult, entermaker, TEXT("\n"));
	bodyresult = regex_replace(bodyresult, golremove, TEXT("\n"));
	
	

	//wstring headcontent = TEXT("HEAD TEXT\n\n");
	//wstring bodycontent = TEXT("\n\nBODY TEXT\n\n");

	totalresult.append(headerresult).append(bodyresult); //.append(bodyresult);
	
	wow = const_cast<wchar_t*>(totalresult.c_str());

	if (cases == 1) { //ÀÏ´Ü CGI ÀÏ¶§¸¸ ÀÌ¹ÌÁö ÆÄÀÏ ¿äÃ»
		Search_jpg_image(result); //gif °¡ ³×ÀÌ¹ö¿¡ ÀÖ±â ¶§¹®¿¡ ¿¡·¯°¡ ³ª´Â °ÍÀÓ
	}
	cases = 0;
}
