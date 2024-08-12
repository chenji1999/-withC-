#include "ClientOrderWork.h"
namespace Client {
	bool GetProblemPNG(SOCKET s) {
		// �������˽�������ɹ�,����ͼƬ��С
		unsigned int PNGSize;
		if (::recv(s, reinterpret_cast<char*>(&PNGSize), sizeof(PNGSize), 0) <= 0) {
			printf("Failed to receive PNG size\n");
			return false;
		}
		// �����ڴ��Խ���ͼƬ����
		char *PNGData = new char[PNGSize];
		if (!PNGData) {
			printf("Failed to malloc memory for PNG\n");
			return false;
		}
		unsigned int charReceivedSize = 0;
		while (charReceivedSize < PNGSize) {
			int result = ::recv(s, PNGData + charReceivedSize, PNGSize - charReceivedSize, 0);
			if (result <= 0)
			{
				printf("Failed to receive PNGData\n");
				return false;
			}
			charReceivedSize += result;
		}

		// �����ļ���
		//std::string currentTimeStr = getCurrentTimeStr();
		//std::string fileName = "problem_" + currentTimeStr + ".png";


		std::string currentProblemNumStr = getProblemNumStr();
		std::string fileName = "problem_" + currentProblemNumStr + ".png";

		// ����ͼƬ���ݵ��ļ�
		std::ofstream outFile(fileName, std::ios::binary);
		if (!outFile) {
			std::cerr << "Failed to open file for writing" << std::endl;
			delete[] PNGData;
			return false;
		}
		outFile.write(PNGData, PNGSize);
		outFile.close();

		std::cout << "ͼƬ�ѽ��ղ�����Ϊ " << fileName << std::endl;

		// �ͷ��ڴ�
		delete[] PNGData;
		return true;
	}
	bool SendResultText(SOCKET s){
		std::cin.get(); // ��ȡһ���ַ����س���

		// �ӱ�׼�����ȡ����ַ���
		
		std::cout << "ȷ����ļ��а�����Ĵ�\n ";
		// �Ӽ������ȡ����
		std::string result = GetClipboardText();
		std::cout << "��������������˷��͵Ĵ�:" << result <<std::endl;
		// ��ȡ�ַ�������
		uint32_t resultSize = static_cast<uint32_t>(result.size());

		// �����ַ�������
		if (send(s, reinterpret_cast<char*>(&resultSize), sizeof(resultSize), 0) == SOCKET_ERROR) {
			std::cerr << "Failed to send result size. Error: " << WSAGetLastError() << std::endl;
			return false;
		}
		// �����ַ�������
		if (send(s, result.c_str(), resultSize, 0) == SOCKET_ERROR) {
			std::cerr << "Failed to send result data. Error: " << WSAGetLastError() << std::endl;
			return false;
		}

		return true;
	}
	std::string getCurrentTimeStr() {
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::tm local_tm;
		localtime_s(&local_tm, &in_time_t);

		std::stringstream ss;
		ss << std::put_time(&local_tm, "%Y%m%d%H%M%S");
		return ss.str();
	}
	std::string getProblemNumStr() {
		static int problemNum = 0;
		problemNum += 1; // ����������
		std::stringstream ss;
		ss << problemNum;
		return ss.str(); // �����ַ�����ʾ�ı��
	}
	std::string GetClipboardText() {
		if (!OpenClipboard(NULL)) {
			return "";
		}

		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData == NULL) {
			CloseClipboard();
			return "";
		}

		char* pszText = static_cast<char*>(GlobalLock(hData));
		if (pszText == NULL) {
			CloseClipboard();
			return "";
		}

		std::string text(pszText);

		GlobalUnlock(hData);
		CloseClipboard();

		return text;
	}
}
