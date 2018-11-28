#include "stdafx.h"
#include "Network.h"

#define CLIENTSNUM 1
using namespace std;

Network::Network()
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("winsock initialize error");



	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit(const_cast<char*>("socket()"));

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("bind()"));

	// listen()

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("listen()"));


}

Network::~Network()
{
}




void Network::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// ���� �Լ� ���� ���
void Network::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void Network::acceptClients()
{
	while (clientNum < CLIENTSNUM)
	{

		// ������ ��ſ� ����� ����

		SOCKADDR_IN clientaddr;
		int addrlen;


		SOCKET client_sock;

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display(const_cast<char*>("accept()"));
			continue;
		}
		//cout << endl << "FileSender ���� : IP �ּ� = " << inet_ntoa(clientaddr.sin_addr) << ", ��Ʈ ��ȣ= " << ntohs(clientaddr.sin_port) << endl;
		Arg arg = { this, clientNum };
//		CreateThread(NULL, 0, this->ServerMain, (LPVOID)clientNum, 0, NULL);
		// ���Ϳ��� client_sock �־��ֱ� 
		ClientSockets.emplace_back(client_sock);
		PTHREAD_START_ROUTINE;
		HANDLE hThread = CreateThread(NULL, 0, &ServerMain, &arg, 0, NULL);
	
		CloseHandle(hThread);
		//clientNum++;
		cout << clientNum++ << endl;
	}

}


DWORD WINAPI Network::ServerMain(LPVOID p)
{

	Arg* pArg = (Arg*)p;
	pArg->p->testFunc(pArg->clientnum);
	return 0;

}

void Network::testFunc(int num)
{
	int clientNum = (int)num;
	SOCKET client_sock = ClientSockets[clientNum];
	while (1) {
		SOCKADDR_IN clientaddr;
		int addrlen;
		addrlen = sizeof(clientaddr);
		getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

		int retval = 0;
		
		KeyData k = { 0, };
		retval = recv(client_sock, (char *)&k, sizeof(k), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char*>("recv()"));
			closesocket(client_sock);
			continue;
		}
		// �����庰�� ���� �ʿ��� (���� 0, 1, 2 ó��) = clientNum 
		// ���� key ť���� �ֱ� 
		cout << k.key << "\t" << k.pressed << "\t" << k.clientNum << endl;
        InputQueue.push(k);
	}

	// closesocket()
	closesocket(client_sock);

}
