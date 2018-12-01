#include "stdafx.h"
#include "Network.h"


Network NW;

Network::Network()
{
	


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
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCSTR)msg, MB_ICONERROR);
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






//DWORD WINAPI Network::ProcessClient(LPVOID p)
//{
//	Arg* pArg = (Arg*)p;
//	pArg->p->testFunc(pArg->sock);
//	
//	return 0;
//
//}

//void Network::recvstart()
//{
//	//Arg arg = { this, clientSock };
//
//
//	//HANDLE hThread = CreateThread(NULL, 0, &ProcessClient, &arg, 0, NULL);
//	
//	HANDLE hThread = CreateThread(NULL, 0, [](LPVOID)->DWORD { NW.testFunc(NW.clientSock); return 0; }, 0, 0, NULL);
//
//	CloseHandle(hThread);
//}
//
//void Network::testFunc(SOCKET sock)
//{
//
//	SOCKADDR_IN clientaddr;
//	
//
//
//	int retval = 0;
//	while (1) {
//
//		int Size = 0;
//		retval = recvn(NW.clientSock, (char*)&Size, sizeof(Size), 0);
//		if (retval == SOCKET_ERROR) {
//			err_display(const_cast<char*>("recv()"));
//			break;
//		}
//
//		DX XMFLOAT3* Pos = new DX XMFLOAT3[Size];
//		retval = recvn(NW.clientSock, (char*)&Pos[0], sizeof(DX XMFLOAT3) * Size, 0);
//		if (retval == SOCKET_ERROR) {
//			err_display(const_cast<char*>("recv()"));
//			break;
//		}
//
//		for (size_t i = 0; i < Size; ++i) {
//			RenderDevice.DrawTexRect(
//				Pos[i],
//				{ 100.f, 100.f },
//				{ 1.f, 1.f, 1.f, 1.f },
//				TEX
//			);
//		}
//	}
//	
//}

void Network::sendInput(KeyData k)
{
	retval = send(clientSock, (char*)&k, sizeof(k), 0);
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
}


void Network::Init()
{
	RenderDevice.Initialize(800, 800);
	TEX = RenderDevice.CreatePngTexture("./Resources/Characters/Isaac.png");


	//NW.Positions.resize(100);

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
	
	//recvstart();
	// 이부분은 메인에서 따로 호출필요 
//	HANDLE hThread = CreateThread(NULL, 0, NW.ProcessClient, (LPVOID)sock, 0, NULL);
}

int Network::recvn(SOCKET s, char * buf, int len, int flags)
{

	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);

}