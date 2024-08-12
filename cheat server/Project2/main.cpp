#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> //����ͷ�ļ� 
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <ctime>
#include "ClientOrderEnum.h"
#include "ClientOrderWork.h"
#pragma comment(lib,"WS2_32.lib") //��ʽ�����׽��ֿ� 
std::string getCurrentTimeStr() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm local_tm;
	localtime_s(&local_tm, &in_time_t);

	std::stringstream ss;
	ss << std::put_time(&local_tm, "%Y%m%d%H%M%S");
	return ss.str();
}
int main() {
	WSADATA data; // ����WSADATA�ṹ�����
	WORD w = MAKEWORD(2, 2); // ����汾����
	if (::WSAStartup(w, &data) != 0) {
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	SOCKET s; // ���������׽��־��
	s = ::socket(AF_INET, SOCK_STREAM, 0); // ����TCP�׽���
	if (s == INVALID_SOCKET) {
		printf("Socket creation failed with error: %d\n", WSAGetLastError());
		::WSACleanup();
		return 1;
	}
	std::string serverIp;
	std::cout << "�����������׵ĵ��Ե�IP: ";
	std::cin >> serverIp;

	sockaddr_in addr; // �����׽��ֵ�ַ�ṹ
	addr.sin_family = AF_INET; // ��ʼ����ַ�ṹ
	addr.sin_port = htons(51107);
	addr.sin_addr.S_un.S_addr = inet_addr(serverIp.c_str());
	printf("�ͻ����Ѿ�����\r\n"); // �����ʾ��Ϣ

	if (::connect(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("Connect failed with error: %d\n", WSAGetLastError());
		::closesocket(s);
		::WSACleanup();
		return 1;
	}
	std::cout << "1:��ʾ������Ŀ����" << std::endl;
	std::cout << "2:��ʾ���Դ�����ı�\n" << std::endl;
	while (true){
		int order;
		std::cout << "�������������֣�" << std::endl;
		std::cin >> order;
		if (send(s, reinterpret_cast<char*>(&order), sizeof(order), 0) == SOCKET_ERROR) {
			printf("Failed to send order\n");
			return 0;
		}
		bool flag = false;
		switch (order)
		{
		case Client::picture: {
			flag = Client::GetProblemPNG(s);
			if (!flag) {
				::closesocket(s);
				::WSACleanup();
				return 0;
			}
			break;
		}
		case Client::result: {
			flag = Client::SendResultText(s);
			if (!flag) {
				::closesocket(s);
				::WSACleanup();
				return 0;
			}
			break;
		}
		default: {
			//::closesocket(s);
			//::WSACleanup();
			break;
		}
		}
	 //   // �������˽�������ɹ�,����ͼƬ��С
		//unsigned int PNGSize;
		//if (::recv(s, reinterpret_cast<char*>(&PNGSize), sizeof(PNGSize), 0) <= 0) {
		//	printf("Failed to receive PNG size\n");
		//	::closesocket(s);
		//	::WSACleanup();
		//	return 0;
		//}
		//// �����ڴ��Խ���ͼƬ����
		//char *PNGData = new char[PNGSize];
		//if (!PNGData){
		//	printf("Failed to malloc memory for PNG\n");
		//	::closesocket(s);
		//	::WSACleanup();
		//	return 0;
		//}
		//unsigned int charReceivedSize=0;
		//while (charReceivedSize < PNGSize) {
		//	int result = ::recv(s, PNGData+charReceivedSize, PNGSize-charReceivedSize, 0);
		//	if (result <= 0)
		//	{
		//		printf("Failed to receive PNGData\n");
		//		::closesocket(s);
		//		::WSACleanup();
		//		return 0;
		//	}
		//	charReceivedSize += result;
		//}
		//
		//// �����ļ���
		//std::string currentTimeStr = getCurrentTimeStr();
		//std::string fileName = "problem_" + currentTimeStr + ".png";

		//// ����ͼƬ���ݵ��ļ�
		//std::ofstream outFile(fileName, std::ios::binary);
		//if (!outFile) {
		//	std::cerr << "Failed to open file for writing" << std::endl;
		//	delete[] PNGData;
		//	closesocket(s);
		//	WSACleanup();
		//	return 1;
		//}
		//outFile.write(PNGData, PNGSize);
		//outFile.close();

		//std::cout << "ͼƬ�ѽ��ղ�����Ϊ " << fileName << std::endl;

		//// �ͷ��ڴ�
		//delete[] PNGData;
	}
	return 0;
}