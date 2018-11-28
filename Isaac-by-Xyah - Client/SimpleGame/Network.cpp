#include "stdafx.h"
#include "Network.h"

Network::Network()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("winsock initialize error");
	//return -1;

 // socket()
	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET) err_quit(const_cast<char*>("socket()"));



	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;

	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(clientSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("connect()"));

	// 이부분은 메인에서 따로 호출필요 
//	HANDLE hThread = CreateThread(NULL, 0, NW.ProcessClient, (LPVOID)sock, 0, NULL);


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

// 소켓 함수 오류 출력
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






DWORD WINAPI Network::ProcessClient(LPVOID p)
{
	Arg* pArg = (Arg*)p;
	pArg->p->testFunc(pArg->sock);
	
	return 0;

}

void Network::start()
{
	Arg arg = { this, clientSock };


	HANDLE hThread = CreateThread(NULL, 0, &ProcessClient, &arg, 0, NULL);
	
	CloseHandle(hThread);
}

void Network::testFunc(SOCKET sock)
{
	SOCKET client_sock = sock;
	SOCKADDR_IN clientaddr;

	int retval = 0;
	while (1) {
		retval = recv(client_sock, (char*)&v, sizeof(v), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char*>("recv()"));
			break;
		}
		else if (retval == 0)
			break;

	}
	
}
