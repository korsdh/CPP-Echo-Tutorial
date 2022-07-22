#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << endl;
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress; // 소켓 주소 구조체

	int serverPort = 9876;
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // 소켓 주소체계
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // IPv4주소
	serverAddress.sin_port = htons(serverPort); // 포트

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect ( )");
	cout << "[현재 상태] connect()\n";

	while (1) {
		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << endl;
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}