#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <ctype.h>
#include <string>
#include <regex>
#include <fcntl.h>
#include <io.h>      //������ ���� �����
#include <stdio.h>
#include <shellapi.h> //for hyperlink
#include <locale.h>
/* GDI PLUS HEADER*/
#include <objidl.h>
#include <gdiplus.h>
//#include <gdiplusgraphics.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "comctl32.lib") 
/* GDI PLUS HEADER END*/

#include <CommCtrl.h>// for progress , status bar, Tooltip
#include "resource.h" //���콺 ������ ��ư �˾� �޴��� ���� ���ҽ� ��� �߰�
#include "vector" 
#include <tchar.h>

//#define PORT_NUM 8979 //CGI PORT NUMBER ��̴�
//#define PORT_NUM 8677 //CGI PORT NUMBER ȿ�ٴ�
//#define PORT_NUM 8090 //CGI PORT NUMBER ��δ�
//#define PORT_NUM 8777 //CGI PORT NUMBER ȿ����
//#define PORT_NUM 8887 // ���δ�

#define MAXLEN 1024
#define TEMP_SIZE 1000000
//#define RBUFSIZE 1000000
#define IDC_MAIN_BUTTON 101
#define BACK_BUTTON 104 //���������� ��ư
#define FRONT_BUTTON 105 //���������� ��ư
#define REFRESH_BUTTON 106 //���ΰ�ħ ��ư
#define FAVORITE_BUTTON 107 //���ã�� ��ư
#define HOME_BUTTON 108 //Ȩ��ư
#define IDC_HOME_EDIT 109 //�����ؽ�Ʈ
#define IDC_MAIN_EDIT 102 //�����ؽ�Ʈ
#define ID_LISTBOX 100 //����Ʈ�ڽ�
#define LINE_BUTTON 110 //��輱
#define FUNC_BUTTON 111 //��� ��ư( ��� ���¹� ���� )
#define VISIT_LOG_DELETE_BUTTON 112 //�湮 ��� ����
#define NEW_WINDOW_BUTTON 113 //�� â ��ư
#define ID_COMBOBOX 114 //�޺� �ڽ�
#define ID_FIRSTCHILD 70000 // ���ϵ� ������ ù ��° ���̵� 70,000 �Ҵ�
#define ID_SECONDCHILD 70001 // ���ϵ� ������ �� ��° ���̵�
#define ID_THIRDCHILD 70002 //���ϵ� ������ �� ��° ���̵�

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
/*������ ���� �־����(�Ʒ�)*/
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma warning(disable:4996)

HWND Main_hWnd;
WNDCLASS WndClass;

int nCmdShow_sub;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubEditProc(HWND, UINT, WPARAM, LPARAM); //for subclassing
LPCWSTR lpszClass = TEXT("�̻����� ��������");
LPCWSTR lpszClass_setting = TEXT("���� ������ ����");

WNDPROC OldEditProc; //for 

HWND EdittextBox;
HWND search_button;
HWND textField;
HWND g_OldEditProc;
HWND g_OldButtonProc;

HWND hProg; //Progress bar
HWND hState; //���¹�(�ϴ�)
HWND back_button; //�ڷΰ��� ��ư
HWND front_button; //�����ΰ��� ��ư
HWND refresh_button; //�����ΰ��� ��ư
HWND favorite_button; //���ã�� ��ư
HWND home_button;
HWND line_button; //��輱
HWND func_button; //��� ��ư
HWND visit_log_delete_button; //�湮 ��� ������ư
HWND hList; //����Ʈ�ڽ�
HWND new_window_button;

/* test combobox */
HWND hCombo;

/* �� â ���� */
HWND hwndChild;

/* �� ��Ʈ�� */
// HWND hTab;
// HWND hStatic;

HINSTANCE g_hInst; /* for progress bar*/
wchar_t textbox_buffer[256] = { 0 };
char temp_addr2[20] = { 0 };
int temp_port2 = 0;

char *dns_query_addr = NULL;

long bufsize = 0; //���� �Ҵ��� ���� ����� ������ ����
char rbuf[MAXLEN] = { 0 };

int clientsocket(char *, int);
using namespace std;

int cases = 0;

int combo_page_count = 0;

std::vector<wstring> vector_page;
std::vector<wstring> vector_all_page;//��� �湮�ߴ� ������
wstring totalresult;
LPCWSTR totalresult2 = TEXT(" ���������� ");
wchar_t * wow = NULL;
char temp[TEMP_SIZE]; //�ӽ÷� ���� ������ �迭 ū ũ�� �Ҵ�.

HMENU hMenu = CreateMenu(), hMenu_setting = CreateMenu(), hMenu_favorite = CreateMenu(), hPopup = CreatePopupMenu(), hPopup_setting = CreatePopupMenu(), hPopup_favorite = CreatePopupMenu(); //�޴��� �������� �����س��� �߰���
int visit_pagenumber = 50001; //���ã�� ��ư �ѹ�

int __cdecl dns(char *hostname)
{
	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	ZeroMemory(&wsaData, sizeof(wsaData));
	int iResult = 0;
	INT iRetval = 0;

	DWORD dwRetval;
	ZeroMemory(&dwRetval, sizeof(dwRetval));

	int i = 1;
	
	struct addrinfo *result;
	ZeroMemory(&result, sizeof(result));
	struct addrinfo *ptr;
	ZeroMemory(&ptr, sizeof(ptr));
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	struct sockaddr_in  *sockaddr_ipv4;
	ZeroMemory(&sockaddr_ipv4, sizeof(sockaddr_ipv4));
	
	//struct sockaddr_in6 *sockaddr_ipv6;
	LPSOCKADDR sockaddr_ip;
	ZeroMemory(&sockaddr_ip, sizeof(sockaddr_ip));

	wchar_t ipstringbuffer[46] = { 0 };
	DWORD ipbufferlength = 46;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//--------------------------------
	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//printf("Calling getaddrinfo with following parameters:\n");
	//printf("\tnodename = %s\n", argv[1]);
	//printf("\tservname (or port) = %s\n\n", argv[2]);

	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the result variable will hold a linked list
	// of addrinfo structures containing response
	// information
	dwRetval = getaddrinfo(hostname, 0, &hints, &result);
	if (dwRetval != 0) {
		printf("getaddrinfo failed with error: %d\n", dwRetval);
		WSACleanup();
		return 1;
	}

	printf("getaddrinfo returned success\n");

	// Retrieve each address and print out the hex bytes
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		printf("getaddrinfo response %d\n", i++);
		printf("\tFlags: 0x%x\n", ptr->ai_flags);
		printf("\tFamily: ");
		switch (ptr->ai_family) {
		case AF_UNSPEC:
			printf("Unspecified\n");
			break;
		case AF_INET:
			printf("AF_INET (IPv4)\n");
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			printf("\tIPv4 address %s\n",
				inet_ntoa(sockaddr_ipv4->sin_addr));
			dns_query_addr = inet_ntoa(sockaddr_ipv4->sin_addr);
			break;
		case AF_INET6:
			printf("AF_INET6 (IPv6)\n");

			// We use WSAAddressToString since it is supported on Windows XP and later
			sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
			// The buffer length is changed by each call to WSAAddresstoString
			// So we need to set it for each iteration through the loop for safety
			ipbufferlength = 46;
			iRetval = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
				ipstringbuffer, &ipbufferlength);
			if (iRetval)
				printf("WSAAddressToString failed with %u\n", WSAGetLastError());
			else
				wcout << "\tIPv6 address" << ipstringbuffer << endl;
			break;
		case AF_NETBIOS:
			printf("AF_NETBIOS (NetBIOS)\n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_family);
			break;
		}
		printf("\tSocket type: ");
		switch (ptr->ai_socktype) {
		case 0:
			printf("Unspecified\n");
			break;
		case SOCK_STREAM:
			printf("SOCK_STREAM (stream)\n");
			break;
		case SOCK_DGRAM:
			printf("SOCK_DGRAM (datagram) \n");
			break;
		case SOCK_RAW:
			printf("SOCK_RAW (raw) \n");
			break;
		case SOCK_RDM:
			printf("SOCK_RDM (reliable message datagram)\n");
			break;
		case SOCK_SEQPACKET:
			printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_socktype);
			break;
		}
		printf("\tProtocol: ");
		switch (ptr->ai_protocol) {
		case 0:
			printf("Unspecified\n");
			break;
		case IPPROTO_TCP:
			printf("IPPROTO_TCP (TCP)\n");
			break;
		case IPPROTO_UDP:
			printf("IPPROTO_UDP (UDP) \n");
			break;
		default:
			printf("Other %ld\n", ptr->ai_protocol);
			break;
		}
		printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
		printf("\tCanonical name: %s\n", ptr->ai_canonname);
	}

	freeaddrinfo(result);
	WSACleanup();

	return 0;
}

void progressbar()//(int Pos)
{
	//if (Pos > 100) Pos = 100;
	//SendMessage(hProg, PBM_SETPOS, Pos, 0);
	SendMessage(hProg, PBM_STEPIT, 0, 0);
	//return curPos; //���� ��ġ�� �޾Ƽ� ���� ��ġ���� ���ش�. �׷��� �ϼ�
}

