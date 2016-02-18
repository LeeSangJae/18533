#include "header.h"
#include "parser.h"
#include "Bitmap.h"
#include "visit_page.h"
#include "favorite_page.h"
#include "hyperlink.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	GdiplusStartupInput gdiplusStartupInput; /* FOR GDI*/
	ULONG_PTR           gdiplusToken; /* GOR GDI*/
	g_hInst = hInstance; /*for progress bar*/

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	
	RegisterClass(&WndClass);
	// WS_HSCROLL , WS_VSCROLL ��ũ�ѹ� �߰�
	Main_hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL , CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, (HMENU)NULL, hInstance, NULL);
	
	ShowWindow(Main_hWnd, nCmdShow);
	UpdateWindow(Main_hWnd);


	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	GdiplusShutdown(gdiplusToken);
	return Message.wParam;
} // WinMain

/*for Scroll bar*/
int xPos = 0, yPos = 0;
int yMax = 1024, xMax = 768; //����Ʈ
/*End Sb */

/* for Wheel Scroll*/
int SumDelta = 0;
int WheelUnit = 0;
/* END WS*/

int Pos = 0;


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	WinProc_sub = hWnd;
	PAINTSTRUCT ps;
	HGDIOBJ hfDefault;
	RECT rt;
	RECT rt2;
	static char str[256];
	int xlnc, ylnc;
	int Lines;
	int nScroll;

	/* �ϴ� ���¹� �κ� */
	int SBPart[4];
	RECT prt;	
	SCROLLINFO si;

	/* �÷��� �˾� �޴� �κ� */
	static COLORREF Color = RGB(255, 0, 0);
	BOOLEAN delresult;

	switch (iMessage) {
	case WM_CREATE:
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopup, "���ã����");
		SetMenu(hWnd, hMenu);
		favorite_page_create(hWnd);
		EdittextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 115, 10, 300, 21, hWnd, (HMENU)IDC_MAIN_EDIT, NULL, NULL);
		hfDefault = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(EdittextBox, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		SendMessage(EdittextBox, WM_SETTEXT, NULL, (LPARAM)"�ּ��Է�â");

		button = CreateWindow("BUTTON", "�̵�", WS_VISIBLE | WS_CHILD | WS_BORDER, 415, 10, 100, 20, hWnd, (HMENU)IDC_MAIN_BUTTON, NULL, NULL);
		SendMessage(button, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		/* ���丮 ��ư */
		
		directory_button = CreateWindow("BUTTON", "�湮��Ϻ���", WS_VISIBLE | WS_CHILD | WS_BORDER, 520, 10, 100, 20, hWnd, (HMENU)IDC_VISITPAGE_BUTTON, NULL, NULL);
		SendMessage(directory_button, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		back_button = CreateWindow("BUTTON", "<-", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 10, 20, 20, hWnd, (HMENU)BACK_BUTTON, NULL, NULL);
		front_button = CreateWindow("BUTTON", "->",WS_VISIBLE | WS_CHILD | WS_BORDER, 30, 10, 20, 20, hWnd, (HMENU)FRONT_BUTTON, NULL, NULL);
		refresh_button = CreateWindow("BUTTON", "���ΰ�ħ", WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 10, 60, 20, hWnd, (HMENU)REFRESH_BUTTON, NULL, NULL);
		SendMessage(refresh_button, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		favorite_button = CreateWindow("BUTTON", "��", WS_VISIBLE | WS_CHILD | WS_BORDER, 622, 10, 20, 20, hWnd, (HMENU)FAVORITE_BUTTON, NULL, NULL);
		SendMessage(favorite_button, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		SetFocus(EdittextBox);
		OldEditProc = (WNDPROC)SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)SubEditProc);

		/* ��ũ�ѹ� �κ� ����*/
		xPos = 0;
		yPos = 0;

		SetScrollRange(hWnd, SB_VERT, 0, yMax, TRUE);
		SetScrollPos(hWnd, SB_VERT, 0, TRUE);
		SetScrollRange(hWnd, SB_HORZ, 0, xMax, TRUE);
		SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &Lines, 0);
		WheelUnit = WHEEL_DELTA / Lines;

		/* �ϴ� ���¹� �κ� ����*/
		InitCommonControls();
		hState = CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", hWnd, 0);
		SBPart[0] = 200;
		SBPart[1] = 500;
		SBPart[2] = 700; //���� ���� ������ �Ҵ�
		SBPart[3] = -1;
		
		SendMessage(hState, SB_SETPARTS, 4, (LPARAM)SBPart);
		SendMessage(hState, SB_SETTEXT, 0, (LPARAM) ""); //Progress bar
		SendMessage(hState, SB_SETTEXT, 1, (LPARAM) ""); //�̹��� ���� �̸� ����Ǵ� ����
		SendMessage(hState, SB_SETTEXT, 2, (LPARAM) ""); //�ٿ�ε����� ��ư
		
		/* ���α׷����� �κ� ���� */
		if (Pos) return 0;
		SendMessage(hState, SB_GETRECT, 0, (LPARAM)&prt);
		hProg = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD | PBS_SMOOTH | WS_VISIBLE, prt.left, prt.top, prt.right - prt.left, prt.bottom - prt.top, hState, NULL, g_hInst, NULL);
		Pos = 0;
		SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendMessage(hProg, PBM_SETPOS, Pos, 0);
		ShowWindow(hProg, SW_SHOW);
	

		//SendMessage(hState, SB_GETRECT, 2, (LPARAM)&prt); //�ι�° ��ĭ ���߿� Ȱ���ϱ�
		break;


		//This is where we change the color of the static to blue. If you only want the cursor
		//to change to a hand when the mouse passes over you don't need this stuff.
	case WM_CTLCOLORSTATIC:
		SetTextColor((HDC)wParam, RGB(0, 0, 255));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LONG)GetStockObject(NULL_BRUSH);

		/* �����۸�ũ �κ�*/
		/*
	case WM_SETCURSOR:
		if ((HWND)wParam == hwndStatic)
		{
			if (colormsg == 1) return TRUE;
			SendMessage(hwndStatus, SB_SETTEXT, 0, (LONG) "Email : chris.witt@mim.com.au");
			SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 1);
			colormsg = 1;
			InvalidateRgn((HWND)hWnd, 0, 1);
			SetCursor(mycur);
			return TRUE;
		}
		//If not on the static, change the font back to normal. You don't have to worry
		//about the cursor, it will change back to the arrow automatically. I use the
		//'colormsg' flag so it doesn't continually set the font when the cursor is moving
		//around, because it causes it to flicker. If you only use the cursor change and
		//don't worry about the font change you don't have to worry.
		if (colormsg == 1)
		{
			colormsg = 0;
			SendMessage(hwndStatus, SB_SETTEXT, 0, 0);
			SendMessage(hwndStatic, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
			InvalidateRgn((HWND)hWnd, 0, 1);
		}
		
		break;
		*/

	/* ���콺 ������ �˾� �޴� */
	case WM_CONTEXTMENU:
		hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup = GetSubMenu(hMenu, 0);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, (short)LOWORD(lParam), (short)HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		break;
		
	case WM_LBUTTONDOWN:
		

		//BitBlt(GetDC(hWnd), 0, 0, 1000, 1000, 0, 0, 0, WHITENESS); //�׳� ȭ���� �Ͼ�Ը� �� �� �ڿ� ��������
		//image_hyperlink_maker(WinProc_sub, "www.daum.net");
		break;

	case WM_SIZE:
		SendMessage(hState, WM_SIZE, wParam, lParam);
		/* ��ũ�ѹ� �κ� */
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 50001:	favorite_clicked(50001);break; case 50002: favorite_clicked(50002);break; case 50003: favorite_clicked(50003);break;
		case 50004:	favorite_clicked(50004);break; case 50005: favorite_clicked(50005);break; case 50006: favorite_clicked(50006);break;
		case 50007:	favorite_clicked(50007);break; case 50008: favorite_clicked(50008);break; case 50009: favorite_clicked(50009);break;
		case 50010:	favorite_clicked(50010);break; case 50011: favorite_clicked(50011);break; case 50012: favorite_clicked(50012);break;

		case IDC_MAIN_BUTTON: // when button is clicked, this will happen: ��ư�κ�
			SendMessage(EdittextBox, WM_GETTEXT, sizeof(textbox_buffer) / sizeof(textbox_buffer[0]), reinterpret_cast<LPARAM>(textbox_buffer));
			if(input_valid_check(textbox_buffer) == 1) // �ּ�üũ�ϰ� dns ������������ ����
				Save_visit_page(textbox_buffer);
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case IDC_VISITPAGE_BUTTON:
			printf("�湮��Ϻ��� ��ư Ŭ��!\n");
			Search_visit_page();
			SetWindowText(Main_hWnd, "�湮��Ϻ���");
			//ShellExecute(NULL, "open", "http://www.google.com", NULL, NULL, SW_SHOWNORMAL);
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case BACK_BUTTON:
			back_button_page(textbox_buffer);
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case FRONT_BUTTON:
			front_button_page(textbox_buffer);
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case REFRESH_BUTTON: //���ΰ�ħ
			if (textbox_buffer[0] == NULL)
				break;
			input_valid_check(textbox_buffer); // �ּ�üũ�ϰ� dns ������������ ����
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case FAVORITE_BUTTON: //���ã�� ���
			favorite_page(textbox_buffer, strlen(textbox_buffer), hWnd);
			break;

		case ID_FILE_40001: //�߶󳻱�
			
			break;
		case ID_FILE_40003: //����
			
			break;
		case ID_FILE_40004: //����
			delresult = DeleteFile(image_file_name); //���� �̱��� ����Ʈ��Ȳ
			if(delresult == TRUE)
				MessageBox(NULL, "���ϻ��� ����!", "���ϻ���!", MB_ICONINFORMATION);
			else
				MessageBox(NULL, "������ �����ϴ�!(�̹� �����ǰų� ���� ����)", "���ϻ�������!", MB_ICONINFORMATION);
			break;
		case ID_FILE_RENAME: //�̸��ٲٱ�
			
			break;
		case ID_FILE_PROPERTY: //�Ӽ�
			
			break;
		}

		//���� üũ�� ����
		if (((HWND)lParam == hwndStatic) && (is_hyper_clicked_possible == 1)) {
			cases = 2; //���̽� 2������ �ϰ� �ؾ� ����ؼ� ��ũ�� ���� �̹����� �޾ƿ� �� ���� //���⼭ ���̽�2�� �����±���.
			clientsocket(temp_addr2, temp_port2); //cases = 2�̱� ������ �׳� �ٷ� ��û�ϸ� �ȴ�.
			InvalidateRect(Main_hWnd, NULL, TRUE);
			UpdateWindow(Main_hWnd);
		}
		break;
		/*
		case WM_CHAR: //����ڷκ��� �׳� �Է¹��� �� ����
		len = strlen(str);
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
		*/

	case WM_PAINT: //����ڷκ��� �Է¹��� ���� ȭ�鿡 �ѷ���
		hdc = BeginPaint(hWnd, &ps);
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		rt2 = { 10-xPos,50-yPos,300,100 };
		DrawText(hdc, hypertext, -1, &rt2, DT_WORDBREAK | DT_NOCLIP);
		//for (i = 0; i < 1024; i += 50) {
		rt = { 100-xPos,100-yPos,1024,768 }; //for�� �ȵ����� �ȴ�.
		DrawText(hdc, wow, -1, &rt, DT_CENTER | DT_WORDBREAK | DT_NOCLIP);
	

		//}
		if(image_exist == 1) //�̹����� ���� ���� �����ش�.
			Draw_Image(hdc, image_file_name, 100 - xPos+200, 100 - yPos+200);

		SendMessage(hState, SB_SETTEXT, 1, (LPARAM)image_file_name); //�ϴ� ���¹ٿ� ���� �̹��� ���� �̸� ���
		EndPaint(hWnd, &ps);
		break;
		

	case WM_MOUSEWHEEL:
		nScroll = 0;
		SumDelta += (short)HIWORD(wParam);
		while (abs(SumDelta) >= WheelUnit) {
			if (SumDelta > 0) {
				nScroll--;
				SumDelta -= WheelUnit;
			}
			else { //��ũ�ѹ� ��������
				nScroll++;
				SumDelta += WheelUnit;
			}
		}
		while (nScroll != 0) {
			if (nScroll > 0) {
				SendMessage(hWnd, WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0);
				nScroll--;
			}
			else {
				SendMessage(hWnd, WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0);
				nScroll++;
			}
		}

		break;

	case WM_HSCROLL: //���ν�ũ��
		xlnc = 0;
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			xlnc = -1;
			break;
		case SB_LINERIGHT:
			xlnc = -20;
			break;
		case SB_PAGELEFT:
			xlnc = -20;
			break;
		case SB_PAGERIGHT:
			xlnc = 20;
			break;
		case SB_THUMBTRACK:
			xlnc = HIWORD(wParam) - xPos;
			break;
		default:
			//InvalidateRect(Main_hWnd, NULL, TRUE);
			//UpdateWindow(Main_hWnd);
			break;
		}
		//���ο� ��ġ�� �ּ��� 0 �̻�
		if (xPos + xlnc < 0)
			xlnc = -xPos;
		//���ο� ��ġ�� �ִ��� xMax ����
		if (xPos + xlnc > xMax)
			xlnc = xMax - xPos;
		//���ο� ��ġ ���
		xPos = xPos + xlnc;
		//��ũ�� ��Ű�� �� ��ġ�� �ٽ� ����Ѵ�.
		ScrollWindow(hWnd, -xlnc, 0, NULL, NULL);
		SetScrollPos(hWnd, SB_HORZ, xPos, TRUE);
		break;

	case WM_VSCROLL: //���� ��ũ��
		ylnc = 0;
		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			ylnc = -1;
			break;
		case SB_LINEDOWN:
			ylnc = 1;
			break;
		case SB_PAGEUP:
			ylnc = -20;
			break;
		case SB_PAGEDOWN:
			ylnc = 20;
			break;
		case SB_THUMBTRACK:
			ylnc = HIWORD(wParam) - yPos;
			break;
		default:
			//InvalidateRect(Main_hWnd, NULL, TRUE);
			//UpdateWindow(Main_hWnd);
			break;
		}
		//���ο� ��ġ�� �ּ��� 0 �̻�
		if (yPos + ylnc < 0)
			ylnc = -yPos;
		//���ο� ��ġ�� �ִ��� yMax ����
		if (yPos + ylnc > yMax)
			ylnc = yMax - yPos;
		//���ο� ��ġ ���
		yPos = yPos + ylnc;
		//��ũ�� ��Ű�� �� ��ġ�� �ٽ� ����Ѵ�.
		ScrollWindow(hWnd, 0, -ylnc, NULL, NULL);
		SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
		MoveWindow(hState, xPos, yPos, LOWORD(lParam), HIWORD(lParam), TRUE); //�ϴ� ���¹� �̵�
		break;

	case WM_DESTROY:
		SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)OldEditProc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}

// ����Ŭ���� ������ ���ν��� - hEdit1�� ���� Ŭ������ ����Ǿ� �ִ�!
LRESULT CALLBACK SubEditProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	RECT rt = { 100,100,1000,1000 }; //�ؽ�Ʈ�� ǥ���� ����
	static BOOL INIT_ERASE_FLAG = TRUE;

	switch (message)
	{
	case WM_KEYDOWN:
		if (INIT_ERASE_FLAG == TRUE)
		{
			SetWindowText(EdittextBox, "");
		}
		INIT_ERASE_FLAG = FALSE;

		if (wparam == VK_RETURN)
		{
			SendMessage(EdittextBox, WM_GETTEXT, sizeof(textbox_buffer) / sizeof(textbox_buffer[0]), reinterpret_cast<LPARAM>(textbox_buffer));
			if (input_valid_check(textbox_buffer) == 1) // �ּ�üũ�ϰ� dns ������������ ����
				Save_visit_page(textbox_buffer);
			InvalidateRect(Main_hWnd, NULL, TRUE);
			UpdateWindow(Main_hWnd);
		}
		break;
	case WM_DESTROY:
		SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)OldEditProc);
		PostQuitMessage(0);
		break;
	}

	// CallWindowProc �Լ� : ������ ������ �޽��� �̿ܿ��� ������ ����Ʈ ������ ������� ó��!
	return (CallWindowProc(OldEditProc, hWnd, message, wparam, lparam));
}


int clientsocket(char * addrs, int portnum)
{
	SOCKET sockfd;
	ZeroMemory(&sockfd, sizeof(sockfd));
	WSADATA wsaData;
	ZeroMemory(&wsaData, sizeof(wsaData));
	struct sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));
	
	int str_len = 0;

	FILE *fp = NULL; //�̹����� ���Ϸ� ����.

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		return 1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(addrs); //���� �ּ� string���� �Է�
	addr.sin_port = htons(portnum); //��Ʈ��ȣ �Է�

	int recvTimeout = 3000;  // 3��.
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout)) != 0)
		return -1;
	
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "�ּ��Է¿���!" << endl;
		wow = "���� ����!\n ������ �������� �ʰų� ���� �ּ��Դϴ�. �ٽ� Ȯ�����ּ���.";
		MessageBox(NULL, "������ �������� �ʰų� ���� �ּ��Դϴ�. �ٽ� Ȯ�����ּ���.", "���� ����!", MB_ICONINFORMATION);
		return -1;
	}
	else {
		printf("������ ����Ǿ����ϴ�!!\n");
	}
	char *p_get = 0;
	if (cases == 0) {
		p_get = "GET / HTTP/1.1\r\n\r\n";
	}
	else if (cases == 1) {
		printf("CGI.... GO!!\n");
		if (temp_port2 == 8677)
			p_get = "GET / HTTP/1.1\r\n\r\n"; //ȿ�ٴ�
		else if(temp_port2 == 8090)
			p_get = "GET /index2.html HTTP/1.1\r\n\r\n"; //��δ�
		else if(temp_port2 == 8979)
			p_get = "GET /pic.html HTTP/1.1\r\n\r\n"; //��̴�

		//p_get = "GET /hello.html HTTP/1.1\r\n\r\n"; //ȿ����
		//p_get = "GET /home/ismean21/webserver/webServer_singleThread/calendar.html HTTP/1.1\r\n\r\n"; //���δ�
	}

	if (cases == 2) {//jpg request message
		//printf("query_jpg_real : %s \n", query_jpg_real);
		//p_get = "GET /cup.bmp HTTP/1.1\r\n\r\n";
		send(sockfd, query_jpg, strlen(query_jpg), 0); //���⼭ �޽��� ��û�ϰ�
		recv(sockfd, rbuf, MAXLEN, 0);
		//printf("%s", rbuf);
		/*��̴�*/
		if (temp_port2 == 8979) {
			recv(sockfd, rbuf, 44, 0);
			printf("%s", rbuf);
		}

		printf("GET IMAGE.... GO!!\n");
		if(fp==NULL)
			fp = fopen(image_file_name, "wb"); //rb�� �ؾ���
	}
	else {
		send(sockfd, p_get, strlen(p_get), 0); //���⼭ �޽��� ��û�ϰ�
	}
	char temp[TEMP_SIZE]; //�ӽ÷� ���� ������ �迭 ū ũ�� �Ҵ�.
	int Pos = 0;
	memset(temp, 0x00, TEMP_SIZE);
	FILE *fp3 = fopen("wow.txt","w");
	//temp = (char*)malloc(MAXLEN*sizeof(char));
	while (1) { //���⼭ �޴´�.
		memset(rbuf, 0x00, MAXLEN);
		str_len = recv(sockfd, rbuf, MAXLEN, 0); //NULL�� �ƴ� ������ ���⼭ ��� �����κ��� �޽��� �ϴ� ����
		Pos += 5;
		if(Pos<80) //���⼭�� ������ �������� ���� 
			progressbar(Pos);
		bufsize += str_len;
		if (str_len <= 0)
			break;
		if (cases == 2) {
			fwrite(rbuf, str_len, 1, fp);
		}
		else {
			printf("%s\n", rbuf);
			fwrite(rbuf, str_len, 1, fp3);
			strncat(temp,rbuf,str_len);
		}
		//temp = (char*)realloc(temp, bufsize + (bufsize*sizeof(char)));
		str_len = 0;
	}
	fclose(fp3);
	if (cases == 2) {
		if(fp!=NULL)
		fclose(fp); //���� �ݾ��ֱ�
		image_exist = 1; //�̹����� �ް� �ִٰ� ǥ���� �����(������)
		printf("jpg case �޸� ����\n");
		closesocket(sockfd);
		memset(temp, 0x00, TEMP_SIZE);
		printf("total bufsize : %d\n", bufsize);
		progressbar(100);
		bufsize = 0;
		cases = 0;
	}
	else {
		is_hyper_clicked_possible = 0;
		closesocket(sockfd);
		parser(temp);
		memset(temp, 0x00, TEMP_SIZE);
		printf("total bufsize : %d\n", bufsize);
		progressbar(100);
		bufsize = 0;
	}
	progressbar(0);
	return 0;
}