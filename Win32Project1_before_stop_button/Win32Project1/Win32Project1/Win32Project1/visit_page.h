#include"header.h"
#include"parser.h"
#include<time.h>
int is_button_clicked = 0; //백버튼이 클릭되었는지 검사 
//프론트 버튼이 클릭된 상태에서 새 주소를 입력하는 것은 아무런 문제가 안되기 때문
wchar_t before_same_page[256] = { 0 };
int lastindex = 0;

void Search_visit_page() {
	totalresult.clear();
	int i = 0;
	int end = 0;
	int listsize = 0;
	end = vector_all_page.size();
	std::wstring result = TEXT("<방문 페이지 목록>\n");
	wchar_t * temp = NULL;
	int get_size = 0;
	
	
	if (end == 0) {
		MessageBox(NULL, TEXT("방문했던 페이지가 없습니다!!"), TEXT("에러!"), MB_ICONINFORMATION);
		return;
	}

	for (auto const& s : vector_all_page) { result += (s + TEXT("\n")); }
		totalresult = result;
	
}


void Save_visit_page(wchar_t * page) {
	if (!wcscmp(page, before_same_page)) {
		return;
	}
	wcscpy(before_same_page,page);
	if (is_button_clicked == 1) {
		int end = 0;
		int real_end = 0;
		real_end = vector_page.size();
		end = lastindex;
		is_button_clicked = 0;
		//중간에서 만약 새로운 페이지 입력하면 앞의 페이지를 다 삭제해주면 된다. (그게 최신페이지가 된다.)
		wchar_t * temp;
		wstring temp_front; //앞 페이지(넘겨줄 것)

		//인덱스 위치의 뒤에있는 페이지들 다 삭제해준다.
		for (int i = real_end - 1; i != lastindex-1; i--) {
			temp = const_cast<wchar_t*>(vector_page[i].c_str());
			wcout << TEXT("삭제중인 페이지 : ") << temp << endl;
			vector_page.pop_back();
		}
	}
	vector_page.push_back(page);

	time_t timer;
	ZeroMemory(&timer, sizeof(timer));
	struct tm t;
	ZeroMemory(&t, sizeof(t));
	wchar_t timestring[100] = { 0 };

	timer = time(NULL); // 현재 시각을 초 단위로 얻기
	localtime_s(&t, &timer); // 초 단위의 시간을 분리하여 구조체에 넣기
	wsprintf(timestring, TEXT("%s [ %d-%d-%d.%d:%d:%d ]"), page, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	vector_all_page.push_back(timestring);
}

void back_button_page(wchar_t * page)
{
	int end = 0;
	int decision = 0;
	if (is_button_clicked == 1)
		end = lastindex + 1;
	else
		end = vector_page.size();

	if (end == 1) {
		MessageBox(NULL, TEXT("마지막 페이지입니다!!"), TEXT("에러!"), MB_ICONINFORMATION);
		return;
	}
	is_button_clicked = 1;
	//현재 페이지 정보를 주면 그 페이지를 앞에서부터 찾아서 바로 뒤의 페이지를 출력해준다.
	wchar_t * temp = NULL;
	wstring temp_before; //이전 페이지(넘겨줄 것)
		

	for (int i = end - 1; i != -1; i--) {
		wcout << vector_page[i] << endl;
		temp = const_cast<wchar_t*>(vector_page[i].c_str());
		
		if (i == 0) {
			decision = 0;
			break;
		}

		if (!wcscmp(page, temp)) {
			temp_before = const_cast<wchar_t*>(vector_page[i - 1].c_str());
			decision = 1;
			lastindex = i;
			break;
		}
	}

	if (decision == 0) {
		MessageBox(NULL, TEXT("마지막 페이지입니다!!"), TEXT("에러!"), MB_ICONINFORMATION);
	}
	else {
		if (hwndStatic != NULL) {
			DestroyWindow(hwndStatic);
			temp_port2 = 0;			
		}
		memset(image_file_name, 0x00, sizeof(image_file_name));
		input_valid_check(temp_before);
	}
}

void front_button_page(wchar_t * page)
{
	int end = 0;
	int vec_size = vector_page.size();
	end = lastindex;

	if (is_button_clicked == 0 || (end == vec_size)) //뒤로가기가 안눌린 상태면 가장 최신 페이지이기 떄문에
	{
		MessageBox(NULL, TEXT("마지막 페이지입니다!!"), TEXT("에러!"), MB_ICONINFORMATION);
		return;
	}

	//현재 페이지 정보를 주면 페이지를 뒤에서부터 찾아서 앞의 페이지를 출력해준다.
	wchar_t * temp = NULL;
	wstring temp_front; //앞 페이지(넘겨줄 것)

	temp = const_cast<wchar_t*>(vector_page[lastindex].c_str());
	temp_front = temp; //이전 페이지 저장
	if (lastindex != (vec_size - 1))
		lastindex++;
	else
		is_button_clicked = 0;

	if (hwndStatic != NULL) {
		DestroyWindow(hwndStatic);
		temp_port2 = 0;
	}
	memset(image_file_name, 0x00, sizeof(image_file_name));
	input_valid_check(temp_front);
}