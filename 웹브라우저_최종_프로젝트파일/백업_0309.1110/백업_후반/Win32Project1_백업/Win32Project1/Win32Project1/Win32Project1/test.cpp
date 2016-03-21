#include "header.h"
#include "parser.h"
#include "Bitmap.h"
#include "visit_page.h"
#include "favorite_page.h"
#include "hyperlink.h"
#include "circular_progressbar_direct2d.h"



int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	GdiplusStartupInput gdiplusStartupInput; /* FOR GDI*/
	ULONG_PTR           gdiplusToken; /* GOR GDI*/
	g_hInst = hInstance; 
	nCmdShow_sub = nCmdShow;
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
	// WS_HSCROLL , WS_VSCROLL 스크롤바 추가
	// WS_CLIPCHILDREN : 윈도우 이동시 깜빡거림을 많이 없애준다. 다시 그릴 때 Child 를 제외하고 그림
	Main_hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL , CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, NULL, (HMENU)NULL, hInstance, NULL);
		
	ShowWindow(Main_hWnd, nCmdShow);
	UpdateWindow(Main_hWnd);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	GdiplusShutdown(gdiplusToken);
	return (int)Message.wParam;
} // WinMain


wchar_t str_temp_array[12][100] = { NULL };
BOOL CALLBACK FAVORITE_DELETE_DLG(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) //즐겨찾기 제거 다이알로그
{
	FILE *fp2 = NULL;
	int buffer[12] = {0 }; //즐겨찾기 최대 12개
	int i = 0, select_num = 0;

	int k = 0, favorite_page_count = 50001, delete_count = 50001;
	switch (iMessage) {
	case WM_INITDIALOG:
		SetWindowText(hDlg, TEXT("즐겨찾기 삭제")); //윈도우 텍스트 문구 변경
		hList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_MULTIPLESEL, 10, 10, 260, 300, hDlg, (HMENU)ID_LISTBOX, g_hInst, NULL);
		if (!(fp2 = fopen("favorite_page.txt", "r"))) {
			return FALSE;
		}

		if (fp2 != NULL) {

			fseek(fp2, 0, SEEK_END);
			int textlen = ftell(fp2);

			fseek(fp2, 0, SEEK_SET); // 오프셋 위치 다시 처음으로 설정

			wchar_t strTemp[255] = { 0 };
			while (!feof(fp2))
			{
				memset(strTemp, 0x00, sizeof(strTemp));
				fgetws(strTemp, sizeof(strTemp), fp2);
				wcscpy(str_temp_array[k], strTemp);
				k++;
				if (wcslen(strTemp) > 0) {
					wcstok(strTemp, TEXT("@"));
					wcout << strTemp << endl;
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)strTemp);
				}
			}
			fclose(fp2);
		}

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: //선택된 항목 제거 버튼
			select_num = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
			if (select_num != 0) {
				fp2 = fopen("favorite_page.txt", "w");
				fclose(fp2);
				SendMessage(hList, LB_GETSELITEMS, 100, (LPARAM)buffer);
				for (i = select_num - 1; i > -1; i--) {
					SendMessage(hList, LB_DELETESTRING, buffer[i], 0); // 50001 ~ 50012
					wcscpy(str_temp_array[buffer[i]], TEXT("DELETE"));
				}
				fp2 = fopen("favorite_page.txt", "w");
				for (int i = 0; i < visit_pagenumber - 50001; i++) {
					DeleteMenu(hPopup_favorite, delete_count, TRUE);
					delete_count++;
					/* WIDE CHARACTER 2BYTE ERROR */
					if(wcsstr(str_temp_array[i], TEXT("DELETE")) == NULL){
						wcstok(str_temp_array[i], TEXT("@"));
						AppendMenu(hPopup_favorite, MF_STRING, favorite_page_count, str_temp_array[i]);
						char* pTemp = NULL;
						int iLen = WideCharToMultiByte(CP_ACP, 0, str_temp_array[i], -1, pTemp, 0, NULL, NULL);
						pTemp = new char[iLen + 1];
						WideCharToMultiByte(CP_ACP, 0, str_temp_array[i], -1, pTemp, iLen, NULL, NULL);
						
						string bufs = to_string(favorite_page_count);
						char *num_temp = const_cast<char*>(bufs.c_str());
						fwrite(pTemp, iLen - 1, 1, fp2);
						fwrite("@", 1, 1, fp2);
						fwrite(num_temp, strlen(num_temp), 1, fp2);
						fwrite("\n", 1, 1, fp2);
						favorite_page_count++;
					}			
				}
				fclose(fp2);
				visit_pagenumber = favorite_page_count;
			}
		
			break;
		case IDC_BUTTON2: //즐겨찾기 모두 삭제
			DestroyMenu(hMenu_favorite);
			DestroyMenu(hPopup_favorite);
			/*즐겨찾기 메뉴 생성*/
			hMenu_favorite = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU3));
			hPopup_favorite = GetSubMenu(hMenu_favorite, 0);
			favorite_page_create(WinProc_sub);
			delete_count = 50001;
			for (int i = 0; i < visit_pagenumber - 50001; i++) {
				DeleteMenu(hPopup_favorite, delete_count, TRUE);
				delete_count++;
			}
			visit_pagenumber = 50001;
			DestroyWindow(hList); //없애고 다시 생성
			hList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_MULTIPLESEL, 10, 10, 260, 300, hDlg, (HMENU)ID_LISTBOX, g_hInst, NULL);
			if (!(fp2 = fopen("favorite_page.txt", "w"))) {
				return FALSE;
			}
			fclose(fp2);
			
			
			break;
		case IDOK:
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK SETTING_STARTING_PAGE_DLG(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[128];
	FILE * fp = fopen("home_save.txt", "r");

	wchar_t strTemp[50] = { 0 };
	memset(strTemp, 0x00, sizeof(strTemp));
	fgetws(strTemp, sizeof(strTemp), fp);

	fclose(fp);
	switch (iMessage) {
	case WM_INITDIALOG:
		SetWindowText(hDlg, TEXT("시작페이지 설정")); //윈도우 텍스트 문구 변경
		SetDlgItemText(hDlg, IDC_EDIT1, strTemp);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_EDIT1, str, 128);
			wcscpy(textbox_buffer, str);
			if (textbox_buffer[0] == '\0') {
				break;
			}
			Save_Home();
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
} 

// Description:
//   Creates a tooltip for an item in a dialog box. 
// Parameters:
//   idTool - identifier of an dialog box item.
//   nDlg - window handle of the dialog box.
//   pszText - string to use as the tooltip text.
// Returns:
//   The handle to the tooltip.
//
HWND CreateToolTip(int toolID, HWND hDlg, PTSTR pszText)
{
	if (!toolID || !hDlg || !pszText)
	{
		return FALSE;
	}
	// Get the window of the tool.
	HWND hwndTool = GetDlgItem(hDlg, toolID);

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL,
		g_hInst, NULL);

	if (!hwndTool || !hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hDlg;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	return hwndTip;
}

/*for Scroll bar*/
int xPos = 0, yPos = 0;
int yMax = 768, xMax = 1024; //디폴트
/*End Sb */

/* for Wheel Scroll*/
int SumDelta = 0;
int WheelUnit = 0;
/* END WS*/

int Pos = 0;
SCROLLINFO si;
LPARAM lParam_sub;


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	WinProc_sub = hWnd;
	static PAINTSTRUCT ps;
	RECT rt;
	RECT rt2;
	lParam_sub = lParam;
	static wchar_t str[256];
	int xlnc, ylnc;
	int Lines;
	int nScroll;

	/* 하단 상태바 부분 */
	int SBPart[4];
	RECT prt;	
	

	/* 플로팅 팝업 메뉴 부분 */
	static COLORREF Color = RGB(255, 0, 0);
	BOOLEAN delresult;

	/* Bitmap 
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	hdc = BeginPaint(hWnd, &ps);


	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
	BitBlt(hdc, 100, 100, 123, 160, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(MyBitmap);
	DeleteDC(MemDC);

	EndPaint(hWnd, &ps);*/


	/* button bitmap */
	HBITMAP hBitmap;

	/* Font */
	HFONT hFont;
	
	/* for visit log button */
	int result_visit_log = 0;

	/* 탭 컨트롤 
	wchar_t *arNum[3] = { TEXT("first page"), TEXT("second page"), TEXT("last page") };
	int iPage = 0;*/
	INITCOMMONCONTROLSEX icex;
	
	/* Combobox */
	int i = 0;

	switch (iMessage) {
	case WM_CREATE:
		// Initialize common controls.
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_TAB_CLASSES;
		InitCommonControlsEx(&icex);

		/*즐겨찾기 메뉴 생성*/
		hMenu_favorite = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU3));
		hPopup_favorite = GetSubMenu(hMenu_favorite, 0);
		favorite_page_create(hWnd);

		/*세팅 메뉴 생성*/
		hMenu_setting = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU2));
		hPopup_setting = GetSubMenu(hMenu_setting, 0);

		back_button = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, 0, 0, 25, 25, hWnd, (HMENU)BACK_BUTTON, g_hInst, NULL);
		CreateToolTip(BACK_BUTTON, hWnd, TEXT("클릭하면 이전 페이지로 이동합니다.")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		SendMessage(back_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		front_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 25, 0, 25, 25, hWnd, (HMENU)FRONT_BUTTON, NULL, NULL);
		CreateToolTip(FRONT_BUTTON, hWnd, TEXT("클릭하면 다음 페이지로 이동합니다.")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		SendMessage(front_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		/*새로고침*/
		refresh_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 50, 0, 25, 25, hWnd, (HMENU)REFRESH_BUTTON, NULL, NULL);
		CreateToolTip(REFRESH_BUTTON, hWnd, TEXT("현재 페이지 새로고침")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		SendMessage(refresh_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		/*콤보박스 (에딧텍스트)*/
		hCombo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 75, 0, 390, 30, hWnd, (HMENU)ID_COMBOBOX, g_hInst, NULL);
		
		EdittextBox = GetWindow(hCombo, GW_CHILD);
		//EdittextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 90, 0, 390, 30, hWnd, (HMENU)IDC_MAIN_EDIT, NULL, NULL);
		CreateToolTip(ID_COMBOBOX, hWnd, TEXT("주소입력 기록 중 하나를 선택하면 해당 페이지로 이동합니다.")); //툴팁 생성
		hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		
		SendMessage(EdittextBox, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(EdittextBox, WM_SETTEXT, NULL, (LPARAM)TEXT("")); //사이즈 변경 가능?
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("방문기록보기"));


		search_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 465, 0, 25, 25, hWnd, (HMENU)IDC_MAIN_BUTTON, NULL, NULL);
		CreateToolTip(IDC_MAIN_BUTTON, hWnd, TEXT("입력한 주소로 이동합니다.")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		SendMessage(search_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
				
				
		home_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 490, 0, 25, 25, hWnd, (HMENU)HOME_BUTTON, NULL, NULL);
		CreateToolTip(HOME_BUTTON, hWnd, TEXT("홈으로 이동합니다.")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		SendMessage(home_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		favorite_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 515, 0, 25, 25, hWnd, (HMENU)FAVORITE_BUTTON, NULL, NULL);
		CreateToolTip(FAVORITE_BUTTON, hWnd, TEXT("즐겨찾기 등록/이동")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		SendMessage(favorite_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		func_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 540, 0, 25, 25, hWnd, (HMENU)FUNC_BUTTON, NULL, NULL);
		CreateToolTip(FUNC_BUTTON, hWnd, TEXT("설정")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		SendMessage(func_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
		
		new_window_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 565, 0, 25, 25, hWnd, (HMENU)NEW_WINDOW_BUTTON, NULL, NULL);
		CreateToolTip(NEW_WINDOW_BUTTON, hWnd, TEXT("새 창을 엽니다.")); //툴팁 생성
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		SendMessage(new_window_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		line_button = CreateWindow(TEXT("BUTTON"), NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 0, 25, 1900, 2, hWnd, (HMENU)LINE_BUTTON, NULL, NULL);
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		SendMessage(line_button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		OldEditProc = (WNDPROC)SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)SubEditProc);

		
		Render(hWnd);

		/* 스크롤바 부분 시작 */
		//xPos = 0;
		//yPos = 0;

		//SetScrollRange(hWnd, SB_VERT, 0, yMax, TRUE);
		//SetScrollPos(hWnd, SB_VERT, 0, TRUE);
		//SetScrollRange(hWnd, SB_HORZ, 0, xMax, TRUE);
		//SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &Lines, 0);
		WheelUnit = WHEEL_DELTA / Lines;

		/* 하단 상태바 부분 시작*/
	
		hState = CreateStatusWindow(WS_CHILD | WS_VISIBLE, TEXT(""), hWnd, 0);
		SBPart[0] = 200;
		SBPart[1] = 500;
		SBPart[2] = -1; //남는 공간 나머지 할당
		
		SendMessage(hState, SB_SETPARTS, 3, (LPARAM)SBPart);
		SendMessage(hState, SB_SETTEXT, 0, (LPARAM) ""); //Progress bar
		SendMessage(hState, SB_SETTEXT, 1, (LPARAM) TEXT("")); //이미지 파일 이름 저장되는 공간
		SendMessage(hState, SB_SETTEXT, 2, (LPARAM) TEXT("")); 
		
		/* 프로그레스바 부분 시작 */
		if (Pos) return 0;
		SendMessage(hState, SB_GETRECT, 0, (LPARAM)&prt);
		hProg = CreateWindowEx(0,PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE, prt.left, prt.top, prt.right - prt.left, prt.bottom - prt.top, hState, NULL, g_hInst, NULL);
		Pos = 0;
		SendMessage(hProg, PBM_SETRANGE | PBS_MARQUEE, 0, MAKELPARAM(0, 100));
		//SendMessage(hProg, PBM_SETPOS, Pos, 0);
		SendMessage(hProg, PBM_SETSTEP, (WPARAM)50, 0); //이걸로 몇씩 증가할건지 설정한다.
		ShowWindow(hProg, SW_SHOW);
		

		/* 탭 컨트롤 부분 시작
		hTab = CreateWindow(WC_TABCONTROL, TEXT(""), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE , 0, 0, 0, 0, hWnd, NULL, g_hInst, NULL);
		hStatic = CreateWindow(WC_STATIC, arNum[0], WS_CHILD | WS_VISIBLE , 0, 0, 0, 0, hWnd, NULL, g_hInst, NULL);
		//WS_BORDER 가는 선 테투리 있는 창 만든다.
		tie.mask = TCIF_TEXT;
		tie.pszText = TEXT("one");

		TabCtrl_InsertItem(hTab, 0, &tie);
	
		tie.pszText = TEXT("two");
		TabCtrl_InsertItem(hTab, 1, &tie);
		tie.pszText = TEXT("three");
		TabCtrl_InsertItem(hTab, 2, &tie);
		*/
		/* 시작 홈 호출 */
		load_Home();

		break;

		//This is where we change the color of the static to blue. If you only want the cursor
		//to change to a hand when the mouse passes over you don't need this stuff.

		/* 탭 컨트롤 (노티파이)*/
		/* https://msdn.microsoft.com/en-us/library/windows/desktop/hh298367(v=vs.85).aspx 
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		
		case TCN_SELCHANGING: 
			//Return value
			//Returns TRUE to prevent the selection from changing, or FALSE to allow the selection to change.
			return FALSE; //이걸 반드시 추가해 줘야 동작한다.
	
		case TCN_SELCHANGE:
			iPage = TabCtrl_GetCurSel(hTab);
			LRESULT result = SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)arNum[TabCtrl_GetCurSel(hTab)]);

			//SetWindowText(hStatic, arNum[TabCtrl_GetCurSel(hTab)]);
			return TRUE;
	
		}
		break;*/

	case WM_CTLCOLORSTATIC:
		SetTextColor((HDC)wParam, RGB(0, 0, 255));
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LONG)GetStockObject(NULL_BRUSH);

	/* 마우스 오른쪽 팝업 메뉴 */
	case WM_CONTEXTMENU:
		hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup = GetSubMenu(hMenu, 0);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, (short)LOWORD(lParam), (short)HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		break;
		
	case WM_LBUTTONDOWN: //test
		//printf("lbutton\n");
		//BitBlt(GetDC(hWnd), 0, 0, 1000, 1000, 0, 0, 0, WHITENESS); //그냥 화면을 하얗게만 할 뿐 뒤에 남아있음
		//image_hyperlink_maker(WinProc_sub, "www.daum.net");
		break;

	case WM_RBUTTONDOWN: //test
		//printf("lbutton\n");
		//BitBlt(GetDC(hWnd), 0, 0, 1000, 1000, 0, 0, 0, WHITENESS); //그냥 화면을 하얗게만 할 뿐 뒤에 남아있음
		//image_hyperlink_maker(WinProc_sub, "www.daum.net");
		break;

	case WM_SIZE:
		/* 탭 컨트롤 부분 
		MoveWindow(hTab, 0, 30, LOWORD(lParam), HIWORD(lParam), TRUE);
		MoveWindow(hStatic, LOWORD(lParam)/2-250, HIWORD(lParam)/2, 500, 25, TRUE);
		*/
		/*
		xMax = 1024 - LOWORD(lParam);
		yMax = 768 - HIWORD(lParam);
		SetScrollRange(hWnd, SB_VERT, 0, yMax, TRUE);
		SetScrollPos(hWnd, SB_VERT, 0, TRUE);
		SetScrollRange(hWnd, SB_HORZ, 0, xMax, TRUE);
		SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
		
		/* 스크롤바 부분 */
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
		si.nMin = 0;
		si.nMax = yMax;
		si.nPage = HIWORD(lParam);
		si.nPos = yPos;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		
		si.nMax = 1024;
		si.nPage = LOWORD(lParam);
		si.nPos = xPos;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);


		SendMessage(hState, WM_SIZE, wParam, lParam);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_BACK:
			back_button_page(textbox_buffer);
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;
		}
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 50001:	favorite_clicked(50001);break; case 50002: favorite_clicked(50002);break; case 50003: favorite_clicked(50003);break;
		case 50004:	favorite_clicked(50004);break; case 50005: favorite_clicked(50005);break; case 50006: favorite_clicked(50006);break;
		case 50007:	favorite_clicked(50007);break; case 50008: favorite_clicked(50008);break; case 50009: favorite_clicked(50009);break;
		case 50010:	favorite_clicked(50010);break; case 50011: favorite_clicked(50011);break; case 50012: favorite_clicked(50012);break;
		
		case ID_COMBOBOX:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)textbox_buffer);

				if (!wcscmp(TEXT("방문기록보기"), textbox_buffer)) { //콤보박스의 방문기록보기를 눌렀을 때
					*textbox_buffer = NULL;
					DestroyWindow(visit_log_delete_button);
					if (image_exist == 1) {
						image_exist = 0;
					}
					if (hwndStatic != NULL) {
						DestroyWindow(hwndStatic);
						temp_port2 = 0;
						hypertext = TEXT("");
					}
					result_visit_log = Search_visit_page();
					if (result_visit_log != -1) {
						visit_log_delete_button = CreateWindow(TEXT("BUTTON"), TEXT("방문기록삭제"), WS_VISIBLE | WS_CHILD | BS_BITMAP, 180, 83, 150, 20, hWnd, (HMENU)VISIT_LOG_DELETE_BUTTON, NULL, NULL);
						SetWindowText(Main_hWnd, TEXT("방문기록보기"));
						InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
						UpdateWindow(Main_hWnd);
					}
					SetFocus(Main_hWnd);
					break;
				}
				if (input_valid_check(textbox_buffer) == 1) { // 주소체크하고 dns 실행할지말지 결정
				}
				InvalidateRect(Main_hWnd, NULL, TRUE);
				UpdateWindow(Main_hWnd);
				SetFocus(Main_hWnd);
				break;
			}
			break;

		case FUNC_BUTTON:
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			GetCursorPos(&pt); //커서의 현재 위치를 받아와야 한다.
			TrackPopupMenu(hPopup_setting, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
			//DestroyMenu(hMenu_setting); //없애지 말것
					
			//AppendMenu(hMenu2, MF_STRING | MF_POPUP, (UINT)hPopup, TEXT("즐겨찾기목록"));
			//AppendMenu(hMenu2, MF_STRING | MF_POPUP, (UINT)hPopup_setting, TEXT("설정"));
			//AppendMenu(hMenu2, MF_STRING | MF_POPUP, (UINT)hPopup_log, TEXT("기록"));
			//AppendMenu(hPopup_setting, MF_STRING, 60000, TEXT("시작페이지설정"));
			//AppendMenu(hPopup_log, MF_STRING, 60001, TEXT("방문기록보기"));
			//SetMenu(hWnd, hMenu2);
			//favorite_page_create(hWnd);
			
			break;


		case FAVORITE_BUTTON: //즐겨찾기 등록
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			GetCursorPos(&pt); //커서의 현재 위치를 받아와야 한다.
			TrackPopupMenu(hPopup_favorite, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
			break;


		case ID_FAVORITE_DELETE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, FAVORITE_DELETE_DLG); //시작페이지 설정 다이알로그 호출
			break;
			

		case IDC_MAIN_BUTTON: // when button is clicked, this will happen: 버튼부분
			SendMessage(EdittextBox, WM_GETTEXT, sizeof(textbox_buffer) / sizeof(textbox_buffer[0]), reinterpret_cast<LPARAM>(textbox_buffer));
			if (textbox_buffer[0] == '\0') {
				break;
			}
			if (input_valid_check(textbox_buffer) == 1) { // 주소체크하고 dns 실행할지말지 결정
				Save_visit_page(textbox_buffer);
			}
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			SetFocus(Main_hWnd);
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

		case REFRESH_BUTTON: //새로고침
			if (textbox_buffer[0] == '\0')
				break;
			input_valid_check(textbox_buffer); // 주소체크하고 dns 실행할지말지 결정
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case HOME_BUTTON:
			load_Home();
			break;

		case ID_SAVE: //파일 저장
			delresult = DeleteFile(image_file_name); //현재 미구현 디폴트상황
			if (delresult == TRUE)
				MessageBox(NULL, TEXT("파일삭제 성공!"), TEXT("파일삭제!"), MB_ICONINFORMATION);
			else
				MessageBox(NULL, TEXT("파일이 없습니다!(이미 삭제되거나 없는 파일)"), TEXT("파일삭제실패!"), MB_ICONINFORMATION);
			break;

		case ID_COPY: //복사
			MessageBox(NULL, TEXT("복사 성공!"), TEXT("파일복사!"), MB_ICONINFORMATION);
			break;

		case ID_REFRESH: //새로고침
			if (textbox_buffer[0] == '\0')
				break;
			input_valid_check(textbox_buffer); // 주소체크하고 dns 실행할지말지 결정
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			break;

		case VISIT_LOG_DELETE_BUTTON: //방문기록 삭제
			//SendMessage(hCombo, CB_GETCOUNT, 0, combo_page_count);
			printf("페이지카운트 : %d\n", combo_page_count);
			
			for (int i = 1; i < combo_page_count + 1; i++) {
				SendMessage(hCombo, CB_DELETESTRING, 1, 0);
				printf("페이지 %d 삭제 중...", i);
			}
			combo_page_count = 0;
			Delete_visit_page();
			DestroyWindow(visit_log_delete_button);
			InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
			UpdateWindow(Main_hWnd);
			MessageBox(NULL, TEXT("방문 기록이 모두 삭제되었습니다!"), TEXT("기록 삭제"), MB_ICONINFORMATION);
			break;

		case ID_VISITLOG: //방문기록 보기
			DestroyWindow(visit_log_delete_button);
			*textbox_buffer = NULL;
			if (image_exist == 1) {
				image_exist = 0;
			}
			if (hwndStatic != NULL) {
				DestroyWindow(hwndStatic);
				temp_port2 = 0;
				hypertext = TEXT("");
			}
			result_visit_log = Search_visit_page();
			if (result_visit_log != -1) {		
				visit_log_delete_button = CreateWindow(TEXT("BUTTON"), TEXT("방문기록삭제"), WS_VISIBLE | WS_CHILD | BS_BITMAP, 180, 83, 150, 20, hWnd, (HMENU)VISIT_LOG_DELETE_BUTTON, NULL, NULL);
				SetWindowText(Main_hWnd, TEXT("방문기록보기"));

				InvalidateRect(Main_hWnd, NULL, WM_ERASEBKGND);
				UpdateWindow(Main_hWnd);
			}
			break;
			

		case ID_SETTING_STARTINGPAGE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, SETTING_STARTING_PAGE_DLG); //시작페이지 설정 다이알로그 호출
			break;

		case ID_FAVORITEPAGE: //즐겨찾기 등록 버튼
			favorite_page(textbox_buffer, wcslen(textbox_buffer), hWnd);
			break;
		
		case NEW_WINDOW_BUTTON: //새 창 열기 버튼 // WS_OVERLAPPEDWINDOW (가장 일반적인 윈도우의 스타일)
			//왜 같이 종료될까??
			hwndChild = CreateWindowEx(0, lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, hWnd, NULL, g_hInst, NULL);
			ShowWindow(hwndChild, SW_SHOW);
			hwndTemp = Main_hWnd;
			Main_hWnd = hwndChild; //child 가 종료했을 때 다시 복구해 줘야하는지 체크해주기
			break;
		}

		//더블 체크드 하자
		if (((HWND)lParam == hwndStatic) && (is_hyper_clicked_possible == 1)) {
			cases = 2; //케이스 2번으로 하고 해야 계속해서 링크를 통해 이미지를 받아올 수 있음 //여기서 케이스2로 돌리는구나.
			clientsocket(temp_addr2, temp_port2); //cases = 2이기 때문에 그냥 바로 요청하면 된다.
			InvalidateRect(Main_hWnd, NULL, TRUE);
			UpdateWindow(Main_hWnd);
		}
		else if (((HWND)lParam == hwndStatic) && (is_hyper_clicked_possible == 2)) {
			cases = 0;
			input_valid_check(textbox_buffer);
			InvalidateRect(Main_hWnd, NULL, TRUE);
			UpdateWindow(Main_hWnd);
		}
		break;

	case WM_PAINT: //사용자로부터 입력받은 것을 화면에 뿌려줌
		hdc = BeginPaint(hWnd, &ps);

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		rt2 = { 49-xPos,45-yPos,300,100 };
		DrawText(hdc, hypertext, -1, &rt2, DT_WORDBREAK | DT_NOCLIP);
	
		rt = { 49-xPos,85-yPos,1024,768 }; //for문 안돌려도 된다.
		DrawText(hdc, wow, -1, &rt, DT_WORDBREAK | DT_NOCLIP | DT_EXTERNALLEADING);
	
		if(image_exist == 1) //이미지가 있을 때만 보여준다.
			Draw_Image(hdc, image_file_name, 100-xPos+200, 95-yPos+180);
			
		SendMessage(hState, SB_SETTEXT, 1, (LPARAM)image_file_name); //하단 상태바에 받은 이미지 파일 이름 출력
	
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
			else { //스크롤바 내려갈때
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

	case WM_HSCROLL: //가로스크롤
		xlnc = 0;
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			xlnc = -1;
			break;
		case SB_LINERIGHT:
			xlnc = 1;
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
		}
		//새로운 위치는 최소한 0 이상
		if (xPos + xlnc < 0)
			xlnc = -xPos;
		//새로운 위치는 최대한 xMax 이하
		if (xPos + xlnc > xMax)
			xlnc = xMax - xPos;
		//새로운 위치 계산
		xPos = xPos + xlnc;
		//스크롤 시키고 썸 위치를 다시 계산한다.
		ScrollWindow(hWnd, -xlnc, 0, NULL, NULL);
		SetScrollPos(hWnd, SB_HORZ, xPos, TRUE);
		MoveWindow(hState, xPos, yPos, LOWORD(lParam), HIWORD(lParam), TRUE); //하단 상태바 이동
		InvalidateRect(Main_hWnd, NULL, TRUE);
		break;

	case WM_VSCROLL: //세로 스크롤
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
		}
		//새로운 위치는 최소한 0 이상
		if (yPos + ylnc < 0)
			ylnc = -yPos;
		//새로운 위치는 최대한 yMax 이하
		if (yPos + ylnc > yMax)
			ylnc = yMax - yPos;
		//새로운 위치 계산
		yPos = yPos + ylnc;
		//스크롤 시키고 썸 위치를 다시 계산한다.
		ScrollWindow(hWnd, 0, -ylnc, NULL, NULL);
		SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
		MoveWindow(hState, xPos, yPos, LOWORD(lParam), HIWORD(lParam), TRUE); //하단 상태바 이동
		break;
		// WM_QUIT 호출 시에는 PostQuitMessage 가 호출된다.

	case WM_CLOSE: // X 버튼을 눌렀을 때 처음 호출된다. 부모일때는 부모만 종료하고 자식일 때는 자식만 종료해야 함
		if (hWnd == hwndChild) { // 자식 윈도우에서 호출한다는 메세지 까지 캐치 완료
			SendMessage(hwndChild, WM_DESTROY, 0, 0);
			break;
		}
		else {
			DestroyWindow(hWnd); // WM_DESTROY 호출해서 모든 윈도우들을 종료해준다.
		}
		break;
	//MSDN 에 있는 DestroyWindow에 대한 설명
	//DestroyWindow automatically destroys the associated child or owned 
	//windows when it destroys the parent or owner window. The function first destroys child or owned windows, and then it destroys the parent or owner window.
		
	case WM_DESTROY:
		if (hWnd == hwndChild) {
			Main_hWnd = hwndTemp;
			DestroyWindow(hwndChild);
			break;
			// chucking WM_DESTROY on the floor
			// means this window will just close,
			// and the other one will stay up.
		}
		else {
			SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)OldEditProc);
			Cleanup(); //원형 프로그레스바 종료
			PostQuitMessage(0);
		}
		//SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)OldEditProc);
		//PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
		break;
	}
}

// 서브클래스 윈도우 프로시저 - hEdit1만 서브 클래싱이 적용되어 있다!
LRESULT CALLBACK SubEditProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static BOOL INIT_ERASE_FLAG = TRUE;

	switch (message)
	{
	case WM_KEYDOWN:
		if (INIT_ERASE_FLAG == TRUE)
		{
			SetWindowText(EdittextBox, TEXT(""));
		}
		INIT_ERASE_FLAG = FALSE;

		switch (wparam) {
		case VK_RETURN:
			{
				SendMessage(EdittextBox, WM_GETTEXT, sizeof(textbox_buffer) / sizeof(textbox_buffer[0]), reinterpret_cast<LPARAM>(textbox_buffer));
				if (textbox_buffer[0] == '\0') {
					break;
				} //명령행 C# + 공백 + 찾을파일 이름 + 스레드 개수 형식으로 준다.
				else if (textbox_buffer[0] == 'C' || textbox_buffer[0] == 'c' && textbox_buffer[1] == '#') { //C# 입력시
					CoInitialize(NULL);
					hr = CoCreateInstance(CLSID_Program,
						NULL, CLSCTX_INPROC_SERVER, IID_Interface_Charp, reinterpret_cast<void**>(&csi));
					if (FAILED(hr))
					{
						printf("C# interface Initialize failed!");
						CoUninitialize();
						return FALSE;
					}

					try {
						csi->initProcess();
					}
					catch (_com_error &e) {
						printf("C# Method call error!");
						return FALSE;
					}

					csi->Release();
					CoUninitialize();
					csi = NULL;
					break;
				}
				if (input_valid_check(textbox_buffer) == 1) { // 주소체크하고 dns 실행할지말지 결정
					Save_visit_page(textbox_buffer);
				}
				InvalidateRect(Main_hWnd, NULL, TRUE);
				UpdateWindow(Main_hWnd);
				SetFocus(Main_hWnd);
				break;
			}
		}
		break;
	

	case WM_DESTROY:
		SetWindowLongPtr(EdittextBox, GWLP_WNDPROC, (LONG_PTR)OldEditProc);
		PostQuitMessage(0);
		break;
	}

	// CallWindowProc 함수 : 관심을 가지는 메시지 이외에는 원래의 에디트 윈도우 방식으로 처리!
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
	
	int curPos_progress = 0; //프로그레스바의 현재 포지션
	int str_len = 0;
	bufsize = 0;
	int dlltlbtest = 0;
	FILE *fp_imagefile = NULL; //이미지를 파일로 저장.
	FILE *fp_recvlog = NULL;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		return -1;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(addrs); //서버 주소 string으로 입력
	addr.sin_port = htons(portnum); //포트번호 입력

	int recvTimeout =3000;  // 3초.
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout)) != 0)
		return -1;
	
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		
		wow = TEXT("접속 에러!\n\n서버가 응답하지 않거나 없는 주소입니다.\n\n다시 확인해주세요.\n\n\n\n\n문제가 해결되지 않으면 아래 링크에서 문제점을 진단하세요.");
		MessageBox(NULL, TEXT("서버가 응답하지 않거나 없는 주소입니다. 다시 확인해주세요."), TEXT("접속 에러!"), MB_ICONINFORMATION);
		cases = 0; //초기화 해 주어야 한다.
		error_hyperlink_maker();
		return -1;
	}

	char *p_get = NULL;
	//p_get = "GET /index.html\r\n HTTP/1.1\r\n"; //기본 널이었던 문제 있었음 //집
	p_get = "GET / HTTP/1.1\r\n\r\n"; //회사

	if (cases == 1) {
		wprintf(TEXT("CGI.... GO!!\n"));
		if (temp_port2 == 8677)
			p_get = "GET / HTTP/1.1\r\n\r\n"; //효근님
		else if(temp_port2 == 8090)
			p_get = "GET /index2.html HTTP/1.1\r\n\r\n"; //재민님
		else if(temp_port2 == 8777)
			p_get ="GET /hello.html HTTP/1.1\r\n\r\n"; //효범님
		//p_get = "GET /home/ismean21/webserver/webServer_singleThread/calendar.html HTTP/1.1\r\n\r\n"; //유민님
	}

	if (cases == 2) {//jpg request message
		send(sockfd, query_jpg, strlen(query_jpg), 0); //여기서 메시지 요청하고
		recv(sockfd, rbuf, MAXLEN, 0);

		wprintf(TEXT("GET IMAGE.... GO!!\n"));
		if (fp_imagefile == NULL) {
			fp_imagefile = _wfopen(image_file_name, TEXT("wb")); //rb로 해야함
		}
	}
	else {
		send(sockfd, p_get, strlen(p_get), 0); //여기서 메시지 요청하고
	}
	char temp[TEMP_SIZE]; //임시로 값을 저장할 배열 큰 크기 할당.

	memset(temp, 0x00, TEMP_SIZE);
	
	if (fp_recvlog == NULL) {
		fp_recvlog = fopen("recv_log.txt", "w");
	}
	while (1) { //여기서 받는다.
		memset(rbuf, 0x00, MAXLEN);
		str_len = recv(sockfd, rbuf, MAXLEN, 0); //NULL이 아닐 때까지 여기서 계속 서버로부터 메시지 일단 받음
	
		progressbar();
		
		if (str_len <= 0)
			break;
		if (cases == 2) { //이미지 파일인 경우
			fwrite(rbuf, str_len, 1, fp_imagefile);
		}
		else {
			strncat(temp, rbuf,str_len);
			bufsize += str_len;
		}
		str_len = 0;
	}
	fwrite(temp, TEMP_SIZE, 1, fp_recvlog);
	
	if (bufsize == 0)
		bufsize = 6;
	else
		yMax = bufsize / 4;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
	si.nMin = 0;
	si.nMax = yMax;
	si.nPage = HIWORD(lParam_sub);
	si.nPos = yPos;
	SetScrollInfo(Main_hWnd, SB_VERT, &si, TRUE);


	if (cases == 2) {
		if (fp_imagefile != NULL)
			fclose(fp_imagefile); //파일 닫아주기
		image_exist = 1; //이미지를 받고 있다고 표시해 줘야함(누르고)
		closesocket(sockfd);
		memset(temp, 0x00, TEMP_SIZE);
		wprintf(TEXT("total bufsize : %d"), bufsize);
		progressbar();
		bufsize = 0;
		cases = 0;
	}
	else {
		is_hyper_clicked_possible = 0;
		closesocket(sockfd);
		wchar_t* temp2 = new wchar_t[TEMP_SIZE];
		if (strstr(temp, "UTF-8") || strstr(temp, "utf-8"))
			MultiByteToWideChar(CP_UTF8, 0, temp, -1, temp2, strlen(temp)*sizeof(TCHAR)); //if utf-8 
		else
			MultiByteToWideChar(CP_ACP, 0, temp, -1, temp2, strlen(temp)*sizeof(TCHAR)); //CP_ACP IS The system default Windows ANSI code page.
		parser(temp2);
		memset(temp, 0x00, TEMP_SIZE);
		delete[] temp2;
		wprintf(TEXT("total bufsize : %d"), bufsize);
		progressbar();
		bufsize = 0;
	}
	SendMessage(hProg, PBM_SETPOS, 0, 0);
	//progressbar();
		
	return 0;
}
