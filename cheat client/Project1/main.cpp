///*
//�Ե��Խ��н������ѽ������ݱ��浽���а�
//*/
//#include <Windows.h>
//#include <stdio.h>
//#include <iostream>
//#include <winsock2.h>
//#include <iostream>
//#include <stdlib.h>
//
//void main()
//{
//
//	//��ȡ�����С
//	HDC hdcScreen = ::GetDC(NULL);
//	//HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
//	int nBitPerPixel = GetDeviceCaps(hdcScreen, BITSPIXEL);
//	int nWidth = GetDeviceCaps(hdcScreen, HORZRES);
//	int nHeight = GetDeviceCaps(hdcScreen, VERTRES);
//
//	//����һ����Ļ�豸�������
//	HDC hMemDC;
//	HBITMAP hBitmap, hOldBitmap;
//	hMemDC = CreateCompatibleDC(hdcScreen);
//	hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
//	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
//
//	//����Ļ�豸�����������ڴ��豸��������
//	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
//
//	//�򿪼����壬����λͼ������������
//	OpenClipboard(NULL);
//	EmptyClipboard();
//	SetClipboardData(CF_BITMAP, hBitmap);
//	CloseClipboard();
//
//	//�ͷ���Դ
//	DeleteDC(hdcScreen);
//	DeleteDC(hMemDC);
//	DeleteObject(hBitmap);
//}


/*
��ȡ���Ծ������ǻػ�IP
*/
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdio.h>
//
//#pragma comment(lib, "Ws2_32.lib")
//
//int main() {
//	WSADATA wsaData;
//	char hostname[256];
//	struct addrinfo hints, *res, *ptr;
//	struct sockaddr_in *sockaddr_ipv4;
//	char ipstr[INET_ADDRSTRLEN];
//	bool found = false;
//
//	// ��ʼ�� Winsock ��
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
//		return 1;
//	}
//
//	// ��ȡ������
//	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
//		printf("gethostname failed with error: %d\n", WSAGetLastError());
//		WSACleanup();
//		return 1;
//	}
//
//	printf("Host name: %s\n", hostname);
//
//	// ���� addrinfo �ṹ
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET; // ֻ���� IPv4 ��ַ
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//
//	// ��ȡ��ַ��Ϣ
//	if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
//		printf("getaddrinfo failed with error: %d\n", WSAGetLastError());
//		WSACleanup();
//		return 1;
//	}
//
//	// ������ַ��Ϣ��Ѱ�ҵ�һ���ǻ��ص�ַ
//	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
//		sockaddr_ipv4 = (struct sockaddr_in *)ptr->ai_addr;
//		inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstr, sizeof(ipstr));
//		if (sockaddr_ipv4->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) {
//			printf("First non-loopback IPv4 Address: %s\n", ipstr);
//			found = true;
//			break;
//		}
//	}
//
//	// ����Ƿ��ҵ���Ч�� IP ��ַ
//	if (!found) {
//		printf("No non-loopback IPv4 Address found.\n");
//	}
//
//	// �ͷŵ�ַ��Ϣ
//	freeaddrinfo(res);
//
//	// ���� Winsock ��
//	WSACleanup();
//
//	return 0;
//}



//����test
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <Windows.h>
//#include <gdiplus.h>
//#include <thread>
//#include <chrono>
//#include <iostream>
//#include <string>
//#include <sstream>
//
//#pragma comment(lib, "Ws2_32.lib") // ��ʽ�����׽��ֿ�
//#pragma comment(lib, "Gdiplus.lib") // ��ʽ����GDI+��
//
//// ��ȡͼ��������� CLSID
//int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
//	UINT num = 0;          // ͼ�������������
//	UINT size = 0;         // ���軺�����Ĵ�С
//
//	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
//
//	Gdiplus::GetImageEncodersSize(&num, &size);
//	if (size == 0) return -1;  // û���ҵ�������
//
//	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
//	if (pImageCodecInfo == NULL) return -1;  // �ڴ治��
//
//	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
//
//	for (UINT j = 0; j < num; ++j) {
//		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
//			*pClsid = pImageCodecInfo[j].Clsid;
//			free(pImageCodecInfo);
//			return j;  // �ɹ�
//		}
//	}
//
//	free(pImageCodecInfo);
//	return -1;  // ʧ��
//}
//
//// ����������Ϊ PNG �ļ�
//void CaptureScreenAndSave(int count) {
//	// ��ʼ�� GDI+
//	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
//	ULONG_PTR gdiplusToken;
//	Gdiplus::Status gdiplusStatus = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
//	if (gdiplusStatus != Gdiplus::Ok) {
//		std::wcerr << L"Failed to initialize GDI+: " << gdiplusStatus << std::endl;
//		return;
//	}
//
//	// ��ȡ���洰�ڵ��豸������
//	HDC hdcScreen = GetDC(NULL);
//	HDC hdcMemory = CreateCompatibleDC(hdcScreen);
//
//	// ��ȡ��Ļ�ߴ�
//	int nWidth = GetDeviceCaps(hdcScreen, HORZRES);
//	int nHeight = GetDeviceCaps(hdcScreen, VERTRES);
//
//	// ��������λͼ
//	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
//	SelectObject(hdcMemory, hBitmap);
//
//	// ����Ļ���ݸ��Ƶ��ڴ��豸������
//	BitBlt(hdcMemory, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);
//
//	// ���� Gdiplus λͼ
//	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(hBitmap, NULL);
//
//	// ��ȡ PNG �������� CLSID
//	CLSID clsidEncoder;
//	if (GetEncoderClsid(L"image/png", &clsidEncoder) == -1) {
//		std::wcerr << L"Failed to get PNG encoder CLSID" << std::endl;
//		delete bitmap;
//		DeleteObject(hBitmap);
//		DeleteDC(hdcMemory);
//		ReleaseDC(NULL, hdcScreen);
//		Gdiplus::GdiplusShutdown(gdiplusToken);
//		return;
//	}
//
//	// �����ļ���
//	std::wstringstream wss;
//	wss << L"screenshot_" << count << L".png";
//	std::wstring filename = wss.str();
//
//	// ����λͼΪ PNG �ļ�
//	Gdiplus::Status status = bitmap->Save(filename.c_str(), &clsidEncoder, NULL);
//	if (status != Gdiplus::Ok) {
//		std::wcerr << L"Failed to save bitmap: " << status << std::endl;
//	}
//	else {
//		std::wcout << L"Screenshot saved as " << filename << std::endl;
//	}
//
//	// �ͷ���Դ
//	delete bitmap;
//	DeleteObject(hBitmap);
//	DeleteDC(hdcMemory);
//	ReleaseDC(NULL, hdcScreen);
//	Gdiplus::GdiplusShutdown(gdiplusToken);
//}
//
//int main() {
//	int count = 0;
//	while (true) {
//		// ����������
//		CaptureScreenAndSave(count++);
//
//		// ÿ����ӽ���һ��
//		std::this_thread::sleep_for(std::chrono::minutes(5));
//	}
//	return 0;
//}




//��������Ŀ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> // �����µĵ�ַת������ͷ�ļ�
#include <stdio.h>
#include <windows.h>
#include "thread_pool.h"
#include "handleClient.h"

#pragma comment(lib, "WS2_32.lib") // �׽��ֿ� ��ʽ����
int main() {
	WSADATA data;
	WORD w = MAKEWORD(2, 2); // ʹ�ø��߰汾�� Winsock
	char sztext[] = "beidou";

	// ��ʼ�� Winsock ��
	if (::WSAStartup(w, &data) != 0) {
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) {
		printf("Socket creation failed with error: %d\n", WSAGetLastError());
		::WSACleanup();
		return 1;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(51107);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Bind failed with error: %d\n", WSAGetLastError());
		::closesocket(listenSocket);
		::WSACleanup();
		return 1;
	}

	if (::listen(listenSocket, 4) == SOCKET_ERROR) {
		printf("Listen failed with error: %d\n", WSAGetLastError());
		::closesocket(listenSocket);
		::WSACleanup();
		return 1;
	}

	printf("�������Ѿ�����\r\n");

	ThreadPool threadPool(4); // �����̳߳أ���������߳���Ϊ4

	while (true) {
		SOCKET clientSocket = ::accept(listenSocket, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET) {
			int err = WSAGetLastError();
			if (err == WSAEINTR) {
				printf("Accept interrupted, continuing...\n");
				continue;
			}
			if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS) {
				printf("Non-blocking accept or operation in progress, continuing...\n");
				continue;
			}
			printf("Accept failed with error: %d\n", err);
			return 1;
		}

		// ʹ���̳߳ش���ͻ�������
		threadPool.enqueue([clientSocket]() {
			try {
				Server::handleClient(clientSocket);
			}
			catch (const std::exception& e) {
				printf("Exception in client handler: %s\n", e.what());
			}
			catch (...) {
				printf("Unknown exception in client handler\n");
			}
			::closesocket(clientSocket);
		});
	}

	::closesocket(listenSocket); // �رռ����׽��־��
	::WSACleanup(); // �ͷ��׽��ֿ�

	return 0;
}