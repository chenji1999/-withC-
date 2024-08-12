#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "handleClient.h"

namespace Server {
	// ��ʼ�� GDI+ ��Ҫ��ȫ�ֱ���
	ULONG_PTR gdiplusToken;
	/*
	������:GetMyIPv4
	����:  �¼�
	���ã���ȡ������IP��ַ�ĺ�����ֻ�Ǹ��û��鿴IP�ṩ����
	*/
	void GetMyIPv4() {
		WSADATA wsaData;
		char hostname[256];
		struct addrinfo hints, *res, *ptr;
		struct sockaddr_in *sockaddr_ipv4;
		char ipstr[INET_ADDRSTRLEN];
		bool found = false;

		// ��ʼ�� Winsock ��
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			printf("WSAStartup failed with error: %d\n", WSAGetLastError());
			return ;
		}

		// ��ȡ������
		if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
			printf("gethostname failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return ;
		}

		printf("Host name: %s\n", hostname);

		// ���� addrinfo �ṹ
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET; // ֻ���� IPv4 ��ַ
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// ��ȡ��ַ��Ϣ
		if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
			printf("getaddrinfo failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return ;
		}

		// ������ַ��Ϣ��Ѱ�ҵ�һ���ǻ��ص�ַ
		for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
			sockaddr_ipv4 = (struct sockaddr_in *)ptr->ai_addr;
			inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstr, sizeof(ipstr));
			if (sockaddr_ipv4->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) {
				printf("First non-loopback IPv4 Address: %s\n", ipstr);
				found = true;
				break;
			}
		}

		// ����Ƿ��ҵ���Ч�� IP ��ַ
		if (!found) {
			printf("No non-loopback IPv4 Address found.\n");
		}

		// �ͷŵ�ַ��Ϣ
		freeaddrinfo(res);

		// ���� Winsock ��
		WSACleanup();
		return;
	}
	void GetResult(SOCKET clientSocket)
	{
		unsigned int resultSize;
		if (::recv(clientSocket, reinterpret_cast<char*>(&resultSize), sizeof(resultSize), 0) <= 0) {
			std::cerr << "Failed to receive result size" << std::endl;
			return;
		}

		// ����һ���㹻��Ļ����������ս��
		char* buffer = new char[resultSize + 1]; // +1 for null terminator
		if (::recv(clientSocket, buffer, resultSize, 0) <= 0) {
			std::cerr << "Failed to receive result" << std::endl;
			delete[] buffer; // ȷ���ͷ��ѷ�����ڴ�
			return;
		}
		buffer[resultSize] = '\0'; // ȷ���ַ�����null��β

		// �����յ��Ľ��ת��Ϊstd::string
		std::string result(buffer);
		delete[] buffer; // �ͷŻ������ڴ�

		// �򿪼�����
		if (!OpenClipboard(NULL)) {
			std::cerr << "Failed to open clipboard" << std::endl;
			return;
		}

		// ��ռ�����
		if (!EmptyClipboard()) {
			std::cerr << "Failed to empty clipboard" << std::endl;
			CloseClipboard();
			return;
		}

		// ����ȫ���ڴ���󲢽�������Ƶ�������
		HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, result.size() + 1);
		if (hClipboardData == NULL) {
			std::cerr << "Failed to allocate global memory" << std::endl;
			CloseClipboard();
			return;
		}

		// ���ַ������Ƶ�ȫ���ڴ�
		char* pClipboardData = static_cast<char*>(GlobalLock(hClipboardData));
		memcpy(pClipboardData, result.c_str(), result.size() + 1);
		GlobalUnlock(hClipboardData);

		// ���ü���������
		if (SetClipboardData(CF_TEXT, hClipboardData) == NULL) {
			std::cerr << "Failed to set clipboard data" << std::endl;
			GlobalFree(hClipboardData);
			CloseClipboard();
			return;
		}

		// �رռ�����
		CloseClipboard();

		std::cout << "Result has been copied to the clipboard" << std::endl;
	}
	// �Ӽ������ȡͼ��
	HBITMAP GetClipboardImage() {
		// ��ȡ�����С
		HDC hdcScreen = ::GetDC(NULL);
		int nWidth = GetDeviceCaps(hdcScreen, HORZRES);
		int nHeight = GetDeviceCaps(hdcScreen, VERTRES);

		// ����һ����Ļ�豸�������
		HDC hMemDC = CreateCompatibleDC(hdcScreen);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		// ����Ļ�豸�����������ڴ��豸��������
		BitBlt(hMemDC, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);

		// �ͷ���Դ
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hdcScreen);
		DeleteDC(hMemDC);

		return hBitmap;
	}

	// ��ȡͼ��������� CLSID
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
		UINT num = 0;          // ͼ�������������
		UINT size = 0;         // ���軺�����Ĵ�С

		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0) return -1;  // û���ҵ�������

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL) return -1;  // �ڴ治��

		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j) {
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // �ɹ�
			}
		}

		free(pImageCodecInfo);
		return -1;  // ʧ��
	}

	// �� HBITMAP ת��Ϊ PNG ��ʽ�����͸��ͻ���
	void SendImageData(SOCKET clientSocket, HBITMAP hBitmap) {
		if (hBitmap == NULL) {
			printf("No image data available\n");
			return;
		}

		// ��ʼ�� GDI+
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok) {
			printf("GdiplusStartup failed\n");
			return;
		}

		{
			// �� HBITMAP ת��Ϊ Gdiplus::Bitmap
			Gdiplus::Bitmap bmp(hBitmap, NULL);

			// ���� IStream �Ա��� PNG ����
			IStream* pStream = NULL;
			if (CreateStreamOnHGlobal(NULL, TRUE, &pStream) != S_OK) {
				printf("CreateStreamOnHGlobal failed\n");
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}

			// ����ͼ�����������
			CLSID clsidEncoder;
			if (GetEncoderClsid(L"image/png", &clsidEncoder) == -1) {
				printf("Failed to get encoder CLSID\n");
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}

			// ��λͼ����Ϊ PNG ��ʽ
			if (bmp.Save(pStream, &clsidEncoder, NULL) != Gdiplus::Ok) {
				printf("Failed to save bitmap to stream\n");
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}

			// ��ȡ PNG ���ݵĴ�С
			STATSTG statstg;
			if (pStream->Stat(&statstg, STATFLAG_DEFAULT) != S_OK) {
				printf("Failed to get stream stat\n");
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}
			ULONG dataSize = statstg.cbSize.LowPart;

			// �����ڴ��Դ洢 PNG ����
			BYTE* pData = new BYTE[dataSize];
			LARGE_INTEGER liZero = {};
			if (pStream->Seek(liZero, STREAM_SEEK_SET, NULL) != S_OK) {
				printf("Failed to seek to beginning of stream\n");
				delete[] pData;
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}
			ULONG bytesRead;
			if (pStream->Read(pData, dataSize, &bytesRead) != S_OK || bytesRead != dataSize) {
				printf("Failed to read from stream\n");
				delete[] pData;
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}

			// ���� PNG ���ݵĴ�С
			if (send(clientSocket, (char*)&dataSize, sizeof(dataSize), 0) == SOCKET_ERROR) {
				printf("Failed to send data size\n");
				delete[] pData;
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}

			// ���� PNG ����
			if (send(clientSocket, (char*)pData, dataSize, 0) == SOCKET_ERROR) {
				printf("Failed to send image data\n");
				delete[] pData;
				pStream->Release();
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}

			// ����
			delete[] pData;
			pStream->Release();
		}

		// �ر� GDI+
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	void handleClient(SOCKET clientSocket) {
		sockaddr_in addr2;
		int n = sizeof(addr2);
		getpeername(clientSocket, (sockaddr*)&addr2, &n);
		printf("%s �Ѿ�������\r\n", inet_ntoa(addr2.sin_addr));
		while (true) {
			// ������������
			int order;
			if (::recv(clientSocket, reinterpret_cast<char*>(&order), sizeof(order), 0) <= 0) {
				printf("Failed to receive data size\n");
				::closesocket(clientSocket);
				return;
			}
			std::cout << "order=" << order << std::endl;
			// ��ȡ������ͼ�񲢷���
			switch (order){
			case Client::picture: {
				HBITMAP hBitmap = GetClipboardImage();
				SendImageData(clientSocket, hBitmap);
				break;
			}
			case Client::result:{
				GetResult(clientSocket);
				break;
			}
			default: {
				break;
			}
			}
		}
		::closesocket(clientSocket); // �رտͻ����׽��־�� 
	}
}