#include "stdafx.h"
#include "Network.h"

#define CLIENTSNUM 3

Network Nw;

Network::Network()
{
	// 윈속 초기화
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

void Network::AcceptClients()
{
	while (EnteredClientsNum < CLIENTSNUM)
	{
		// 데이터 통신에 사용할 변수
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

		ClientSockets.emplace_back(client_sock);

		retval = send(client_sock, (char*)&EnteredClientsNum, sizeof(EnteredClientsNum), 0);
		EnteredClientsNum++;
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
		PTHREAD_START_ROUTINE;
		HANDLE hThread = CreateThread(NULL, 0, [](LPVOID)->DWORD {Nw.testFunc(); return 0; }, 0, 0, NULL);

		CloseHandle(hThread);

	}

	//OBJ::PLAYER = Engine.AddObject();
	//auto& ActorPhysics = Engine.GetPhysics(OBJ::PLAYER);
	//ActorPhysics.SetPosition(5, 5, 0);
}

void Network::SendRenderData()
{
	//for (auto client_sock : ClientSockets) {
	//	SOCKADDR_IN clientaddr;
	//	int addrlen;
	//	addrlen = sizeof(clientaddr);
	//	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	//	int vecSize = NW.Positions.size();

	//	retval = send(client_sock, (char*)&vecSize, sizeof(vecSize), 0);
	//	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));

	//	retval = send(client_sock, (char*)&NW.Positions[0], sizeof(DX XMVECTOR) * NW.Positions.size(), 0);
	//	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));

	//	//      retval = send(client_sock, (char*)&RenderDataPacket, sizeof(RenderDataPacket), 0);
	//	//      if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));

	//	   //   retval = send(client_sock, (char*)&RenderDataPacket, sizeof(RenderDataPacket), 0);
	//	   //   if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));

	//}
	//Positions.clear();


	for (auto client_sock : ClientSockets) {
		SOCKADDR_IN clientaddr;
		int addrlen;

		addrlen = sizeof(clientaddr);
		getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

		int vecSize = rendererData.size();

		retval = send(client_sock, (char*)&vecSize, sizeof(vecSize), 0);
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));

		
		retval = send(client_sock, (char*)&rendererData[0], sizeof(RenderData) * rendererData.size(), 0);
		if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
	}
	rendererData.clear();
}


//DWORD WINAPI Network::ServerMain(LPVOID p)
//{
//
//	Arg* pArg = (Arg*)p;
//	pArg->p->testFunc(pArg->clientnum);
//	return 0;
//
//}

void Network::testFunc()
{
	SOCKET client_sock = ClientSockets[CurrentClientNum++];
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
		// 스레드별로 변수 필요함 (순서 0, 1, 2 처럼) = clientNum 
		// 받은 key 큐에다 넣기 
		STD cout << k.key << "\t" << k.pressed << "\t" << k.clientNum << STD endl;
        InputQueue.push(k);
	}

	// closesocket()
	closesocket(client_sock);

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