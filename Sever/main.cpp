#include <iostream>
#include <WinSock2.h>
#include <string>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << endl;
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ShowErrorMessage("WSAStartup()");
	}
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET, 0)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		ShowErrorMessage("listen()");
	}
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메시지]: " << received << endl;
		cout << "[메시지 전송]: " << received << endl;
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]" << endl;
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}