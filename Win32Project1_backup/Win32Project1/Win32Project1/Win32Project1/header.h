#pragma once

#undef UNICODE

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
/* GDI PLUS HEADER*/
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusgraphics.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment(lib, "ws2_32.lib") 
/* GDI PLUS HEADER END*/

#include <CommCtrl.h>// for progress , status bar

#include "resource.h" //���콺 ������ ��ư �˾� �޴��� ���� ���ҽ� ��� �߰�
#include "vector" 


//#define PORT_NUM 8979 //CGI PORT NUMBER ��̴�
#define PORT_NUM 8677 //CGI PORT NUMBER ȿ�ٴ�
//#define PORT_NUM 8090 //CGI PORT NUMBER ��δ�
//#define PORT_NUM 8777 //CGI PORT NUMBER ȿ����
//#define PORT_NUM 8887 // ���δ�
#define MAXLEN 1024
//#define RBUFSIZE 1000000
#define IDC_MAIN_BUTTON 101
#define IDC_VISITPAGE_BUTTON 103 //�ٿ�ε� ��ư
#define BACK_BUTTON 104 //���������� ��ư
#define FRONT_BUTTON 105 //���������� ��ư
#define REFRESH_BUTTON 106 //���ΰ�ħ ��ư
#define FAVORITE_BUTTON 107 //���ã�� ��ư
#define IDC_MAIN_EDIT 102 //�����ؽ�Ʈ
#define ID_LISTBOX 100 //���丮 �˻�


#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma warning(disable:4996)

HWND Main_hWnd;

WNDCLASS WndClass;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubEditProc(HWND, UINT, WPARAM, LPARAM); //for subclassing
LPCTSTR lpszClass = "�̻����� ��������";

SOCKET sockfd;
WSADATA wsaData;

WNDPROC OldEditProc; //for subclassing

struct sockaddr_in addr;

static HWND EdittextBox;
static HWND button;
static HWND textField;
static HWND g_OldEditProc;
static HWND g_OldButtonProc;

HWND hProg; //Progress bar
HWND hState; //���¹�(�ϴ�)
HWND hList; //���丮 Ž��
HWND directory_button; //���丮 ��ư
HWND back_button; //�ڷΰ��� ��ư
HWND front_button; //�����ΰ��� ��ư
HWND refresh_button; //�����ΰ��� ��ư
HWND favorite_button; //���ã�� ��ư

HINSTANCE g_hInst; /* for progress bar*/
char textbox_buffer[256];

char *cgi_addr = "52.192.132.151";
char *local_addr = "127.0.0.1";

char *dns_query_addr;

long bufsize; //���� �Ҵ��� ���� ����� ������ ����
char rbuf[MAXLEN+1];
char temp[1000000]; //�ӽ÷� ���� ������ �迭 ū ũ�� �Ҵ�.


int clientsocket(char *, int);
using namespace std;

int cases = 0;

std::vector<string> vector_page;
std::vector<string> vector_all_page;//��� �湮�ߴ� ������
char *totalresult;

HMENU hMenu = CreateMenu(), hPopup = CreatePopupMenu(); //�޴��� �������� �����س��� �߰���
int visit_pagenumber = 50001; //���ã�� ��ư �ѹ�

int __cdecl dns(char *hostname)
{
	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	int iResult;
	INT iRetval;

	DWORD dwRetval;

	int i = 1;

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	struct sockaddr_in  *sockaddr_ipv4;
	//    struct sockaddr_in6 *sockaddr_ipv6;
	LPSOCKADDR sockaddr_ip;

	char ipstringbuffer[46];
	DWORD ipbufferlength = 46;

	/*
	// Validate the parameters
	if (argc != 3) {
	printf("usage: %s <hostname> <servicename>\n", argv[0]);
	printf("getaddrinfo provides protocol-independent translation\n");
	printf("   from an ANSI host name to an IP address\n");
	printf("%s example usage\n", argv[0]);
	printf("   %s www.contoso.com 0\n", argv[0]);
	return 1;
	}
	*/

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
			// the InetNtop function is available on Windows Vista and later
			// sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
			// printf("\tIPv6 address %s\n",
			//    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );

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
				printf("\tIPv6 address %s\n", ipstringbuffer);
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

void progressbar(int Pos)
{
	if (Pos > 100) Pos = 100;
	SendMessage(hProg, PBM_SETPOS, Pos, 0);
}





/* �̺κ� �Q�� -> �湮��� ����� ����
hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_SORT, 10, 10, 10, 10, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
MyFindFile();
void MyFindFile()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	
	BOOL bResult = TRUE;
	TCHAR Path[MAX_PATH];
	TCHAR fname[MAX_PATH];

	GetWindowsDirectory(Path, MAX_PATH);
	lstrcat(Path, "\\*.*");
	hSrch = FindFirstFile(Path, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) return;
	while (bResult) {
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			wsprintf(fname, "[ %s ]", wfd.cFileName);
		}
		else {
			wsprintf(fname, "%s", wfd.cFileName);
		}
		
		//SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)fname);
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);
}

void SetStatusText(HWND hWnd)
{
	RECT wrt;
	TCHAR str[128];
	GetWindowRect(hWnd, &wrt);
	wsprintf(str, "Left: %d", wrt.left);
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)str);
	wsprintf(str, "Top: %d", wrt.top);
	SendMessage(hState, SB_SETTEXT, 1, (LPARAM)str);
	wsprintf(str, "Right: %d", wrt.right);
	SendMessage(hState, SB_SETTEXT, 2, (LPARAM)str);
	wsprintf(str, "Bottom : %d", wrt.bottom);
	SendMessage(hState, SB_SETTEXT, 3, (LPARAM)str);
}*/

