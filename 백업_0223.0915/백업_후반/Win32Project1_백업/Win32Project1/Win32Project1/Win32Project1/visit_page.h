#include"header.h"
#include"parser.h"
#include<time.h>
int is_button_clicked = 0; //���ư�� Ŭ���Ǿ����� �˻� 
//����Ʈ ��ư�� Ŭ���� ���¿��� �� �ּҸ� �Է��ϴ� ���� �ƹ��� ������ �ȵǱ� ����
wchar_t before_same_page[256] = { 0 };
int lastindex = 0;

void Search_visit_page() {
	totalresult.clear();
	int i = 0;
	int end = 0;
	int listsize = 0;
	end = vector_all_page.size();
	std::wstring result = TEXT("<�湮 ������ ���>\n");
	wchar_t * temp = NULL;
	int get_size = 0;
	
	
	if (end == 0) {
		MessageBox(NULL, TEXT("�湮�ߴ� �������� �����ϴ�!!"), TEXT("����!"), MB_ICONINFORMATION);
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
		//�߰����� ���� ���ο� ������ �Է��ϸ� ���� �������� �� �������ָ� �ȴ�. (�װ� �ֽ��������� �ȴ�.)
		wchar_t * temp;
		wstring temp_front; //�� ������(�Ѱ��� ��)

		//�ε��� ��ġ�� �ڿ��ִ� �������� �� �������ش�.
		for (int i = real_end - 1; i != lastindex-1; i--) {
			temp = const_cast<wchar_t*>(vector_page[i].c_str());
			wcout << TEXT("�������� ������ : ") << temp << endl;
			vector_page.pop_back();
		}
	}
	vector_page.push_back(page);

	time_t timer;
	ZeroMemory(&timer, sizeof(timer));
	struct tm t;
	ZeroMemory(&t, sizeof(t));
	wchar_t timestring[100] = { 0 };

	timer = time(NULL); // ���� �ð��� �� ������ ���
	localtime_s(&t, &timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
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
		MessageBox(NULL, TEXT("������ �������Դϴ�!!"), TEXT("����!"), MB_ICONINFORMATION);
		return;
	}
	is_button_clicked = 1;
	//���� ������ ������ �ָ� �� �������� �տ������� ã�Ƽ� �ٷ� ���� �������� ������ش�.
	wchar_t * temp = NULL;
	wstring temp_before; //���� ������(�Ѱ��� ��)
		

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
		MessageBox(NULL, TEXT("������ �������Դϴ�!!"), TEXT("����!"), MB_ICONINFORMATION);
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

	if (is_button_clicked == 0 || (end == vec_size)) //�ڷΰ��Ⱑ �ȴ��� ���¸� ���� �ֽ� �������̱� ������
	{
		MessageBox(NULL, TEXT("������ �������Դϴ�!!"), TEXT("����!"), MB_ICONINFORMATION);
		return;
	}

	//���� ������ ������ �ָ� �������� �ڿ������� ã�Ƽ� ���� �������� ������ش�.
	wchar_t * temp = NULL;
	wstring temp_front; //�� ������(�Ѱ��� ��)

	temp = const_cast<wchar_t*>(vector_page[lastindex].c_str());
	temp_front = temp; //���� ������ ����
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