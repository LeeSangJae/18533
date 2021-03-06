#include"header.h"
#include"parser.h"
#include<time.h>
int is_button_clicked; //백버튼이 클릭되었는지 검사 
//프론트 버튼이 클릭된 상태에서 새 주소를 입력하는 것은 아무런 문제가 안되기 때문
char * before_page;
char before_same_page[256];
int lastindex = 0;

void Search_visit_page() {
	int i = 0;
	int end = 0;
	int listsize = 0;
	end = vector_all_page.size();
	std::string result = "<방문 페이지 목록>\n";
	char * temp = NULL;
	int get_size = 0;
	
	
	if (end == 0) {
		MessageBox(NULL, "방문했던 페이지가 없습니다!!", "에러!", MB_ICONINFORMATION);
		return;
	}

	for (auto const& s : vector_all_page) { result += (s + "\n"); }
	temp = const_cast<char*>(result.c_str());
	get_size = strlen(temp);
	totalresult = new char[get_size];
	sprintf(totalresult, "%s", temp);
}


void Save_visit_page(char * page) {
	if (!strcmp(page, before_same_page)) {
		return;
	}
	strcpy(before_same_page,page);
	if (is_button_clicked == 1) {
		int end = 0;
		int real_end = 0;
		real_end = vector_page.size();
		end = lastindex;
		is_button_clicked = 0;
		//중간에서 만약 새로운 페이지 입력하면 앞의 페이지를 다 삭제해주면 된다. (그게 최신페이지가 된다.)
		char * temp;
		string temp_front; //앞 페이지(넘겨줄 것)

		//인덱스 위치의 뒤에있는 페이지들 다 삭제해준다.
		for (int i = real_end - 1; i != lastindex-1; i--) {
			temp = const_cast<char*>(vector_page[i].c_str());
	
			printf("삭제중인 페이지 : %s\n", temp);
			vector_page.pop_back();
			
		}
	}
	vector_page.push_back(page);

	time_t timer;
	struct tm t;
	char timestring[100];

	timer = time(NULL); // 현재 시각을 초 단위로 얻기
	localtime_s(&t, &timer); // 초 단위의 시간을 분리하여 구조체에 넣기
	sprintf(timestring, "%s [ %d-%d-%d.%d:%d:%d ]", page, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	vector_all_page.push_back(timestring);
}

void back_button_page(char * page)
{
	int end = 0;
	int decision = 0;
	if (is_button_clicked == 1)
		end = lastindex + 1;
	else
		end = vector_page.size();

	if (end == 1) {
		MessageBox(NULL, "마지막 페이지입니다!!", "에러!", MB_ICONINFORMATION);
		return;
	}
	is_button_clicked = 1;
	//현재 페이지 정보를 주면 그 페이지를 앞에서부터 찾아서 바로 뒤의 페이지를 출력해준다.
	char * temp;
	string temp_before; //이전 페이지(넘겨줄 것)
		

	for (int i = end - 1; i != -1; i--) {
		cout << vector_page[i] << endl;
		temp = const_cast<char*>(vector_page[i].c_str());
		
		if (i == 0) {
			decision = 0;
			break;
		}

		if (!strcmp(page, temp)) {
			temp_before = const_cast<char*>(vector_page[i - 1].c_str());
			decision = 1;
			lastindex = i;
			break;
		}
	}

	if (decision == 0) {
		MessageBox(NULL, "마지막 페이지입니다!!", "에러!", MB_ICONINFORMATION);
	}
	else {
		if (hwndStatic != NULL) {
			DestroyWindow(hwndStatic);
			hwndStatic = NULL;
			temp_port2 = 0;			
		}
		image_file_name = NULL;
		input_valid_check(temp_before);
		temp = const_cast<char*>(temp_before.c_str());
		before_page = new char[strlen(temp)];
		sprintf(before_page, "%s", temp);
	}


}

void front_button_page(char * page)
{
	int end = 0;
	int vec_size = vector_page.size();
	end = lastindex;
	
	if (is_button_clicked == 0 || (end == vec_size)) //뒤로가기가 안눌린 상태면 가장 최신 페이지이기 떄문에
	{
		MessageBox(NULL, "마지막 페이지입니다!!", "에러!", MB_ICONINFORMATION);
		return;
	}

	//현재 페이지 정보를 주면 페이지를 뒤에서부터 찾아서 앞의 페이지를 출력해준다.
	char * temp;
	string temp_front; //앞 페이지(넘겨줄 것)
	
	temp = const_cast<char*>(vector_page[lastindex].c_str());
	temp_front = temp; //이전 페이지 저장
	if(lastindex != (vec_size-1))
		lastindex++;
	else
		is_button_clicked = 0;

	if (hwndStatic != NULL) {
		DestroyWindow(hwndStatic);
		hwndStatic = NULL;
		temp_port2 = 0;
	}
	image_file_name = NULL;
	input_valid_check(temp_front);
	//vector<string>::reverse_iterator it = find(vector_page.rbegin(), vector_page.rend(), page);
	//cout <<*it << endl;
}
