#include"header.h"
#include"parser.h"
#include<time.h>
int is_button_clicked = 0; //���ư�� Ŭ���Ǿ����� �˻� 
//����Ʈ ��ư�� Ŭ���� ���¿��� �� �ּҸ� �Է��ϴ� ���� �ƹ��� ������ �ȵǱ� ����
char * before_page = NULL;
char before_same_page[256] = { 0 };
int lastindex = 0;

void Search_visit_page() {
	totalresult.clear();
	int i = 0;
	int end = 0;
	int listsize = 0;
	end = vector_all_page.size();
	std::string result = "<�湮 ������ ���>\n";
	char * temp = NULL;
	int get_size = 0;
	
	
	if (end == 0) {
		MessageBox(NULL, "�湮�ߴ� �������� �����ϴ�!!", "����!", MB_ICONINFORMATION);
		return;
	}

	for (auto const& s : vector_all_page) { result += (s + "\n"); }
		totalresult = result;
	
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
		//�߰����� ���� ���ο� ������ �Է��ϸ� ���� �������� �� �������ָ� �ȴ�. (�װ� �ֽ��������� �ȴ�.)
		char * temp;
		string temp_front; //�� ������(�Ѱ��� ��)

		//�ε��� ��ġ�� �ڿ��ִ� �������� �� �������ش�.
		for (int i = real_end - 1; i != lastindex-1; i--) {
			temp = const_cast<char*>(vector_page[i].c_str());
	
			printf("�������� ������ : %s\n", temp);
			vector_page.pop_back();
		}
	}
	vector_page.push_back(page);

	time_t timer;
	ZeroMemory(&timer, sizeof(timer));
	struct tm t;
	ZeroMemory(&t, sizeof(t));
	char timestring[100] = { 0 };

	timer = time(NULL); // ���� �ð��� �� ������ ���
	localtime_s(&t, &timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
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
		MessageBox(NULL, "������ �������Դϴ�!!", "����!", MB_ICONINFORMATION);
		return;
	}
	is_button_clicked = 1;
	//���� ������ ������ �ָ� �� �������� �տ������� ã�Ƽ� �ٷ� ���� �������� ������ش�.
	char * temp = NULL;
	string temp_before; //���� ������(�Ѱ��� ��)
		

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
		MessageBox(NULL, "������ �������Դϴ�!!", "����!", MB_ICONINFORMATION);
	}
	else {
		if (hwndStatic != NULL) {
			DestroyWindow(hwndStatic);
			temp_port2 = 0;			
		}
		memset(image_file_name, 0x00, sizeof(image_file_name));
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

	if (is_button_clicked == 0 || (end == vec_size)) //�ڷΰ��Ⱑ �ȴ��� ���¸� ���� �ֽ� �������̱� ������
	{
		MessageBox(NULL, "������ �������Դϴ�!!", "����!", MB_ICONINFORMATION);
		return;
	}

	//���� ������ ������ �ָ� �������� �ڿ������� ã�Ƽ� ���� �������� ������ش�.
	char * temp = NULL;
	string temp_front; //�� ������(�Ѱ��� ��)

	temp = const_cast<char*>(vector_page[lastindex].c_str());
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