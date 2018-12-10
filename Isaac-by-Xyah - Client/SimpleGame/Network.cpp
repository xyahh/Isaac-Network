#include "stdafx.h"
#include "Network.h"

Network Nw;

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

void Network::SendInput(KeyData k)
{
	retval = send(clientSock, (char*)&k, sizeof(k), 0);
	if (retval == SOCKET_ERROR) err_quit(const_cast<char*>("send()"));
}


void Network::Init()
{
	RenderDevice.Initialize(800, 800);
	TEX[0] = RenderDevice.GenerateTexture("./Resources/Characters/basic_body.png");
	TEX[1] = RenderDevice.GenerateTexture("./Resources/Characters/cain_head.png");
	TEX[2] = RenderDevice.GenerateTexture("./Resources/explosion.png");
	TEX[3] = RenderDevice.GenerateTexture("./Resources/tear.png");
	TEX[4] = RenderDevice.GenerateTexture("./Resources/Levels/Depths.png");

	//NW.Positions.resize(100);

	// ���� �ʱ�ȭ
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

	retval = recv(clientSock, (char *)&ClientNum, sizeof(ClientNum), 0);
	if (retval == SOCKET_ERROR) {
		err_display(const_cast<char*>("recv()"));
		closesocket(clientSock);
		exit(-1);
	}
	printf("clientNum : %d\n", ClientNum);

	//#pragma region �ֿ����ڵ�
	//UserData player;
	//player.sceneNumber = 0;

	//int menu = 0;
	//bool hasActorNumber = false;
	//bool gameStart = false;;

	//while (1) {
	//	if (player.sceneNumber == LOGINSCENE) 
	//	{
	//		if (gameStart)
	//			return;

	//		STD cout << "�޴��� �����ϼ���" << STD endl;
	//		STD cout << "1. ����" << STD endl;
	//		STD cout << "2. �α���" << STD endl;
	//		STD cin >> menu;

	//		if (menu == SIGNUP)
	//		{
	//			system("cls");
	//			STD cout << "�̸��� �Է��ϼ��� : ";
	//			STD cin >> player.name;
	//			STD cout << "�г����� �Է��ϼ��� : ";
	//			STD cin >> player.nickname;
	//			STD cout << "��й�ȣ�� �Է��ϼ��� : ";
	//			STD cin >> player.password;
	//			player.loginState = 1;
	//		}

	//		else if (menu == SIGNIN)
	//		{
	//			system("cls");
	//			STD cout << "�г����� �Է��ϼ��� : ";
	//			STD cin >> player.nickname;
	//			STD cout << "��й�ȣ�� �Է��ϼ��� : ";
	//			STD cin >> player.password;
	//			player.loginState = 2;
	//		}

	//		retval = send(clientSock, (char *)&player, sizeof(player), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display((char*)"send()");
	//			exit(1);
	//		}

	//		retval = recvn(clientSock, (char *)&player, sizeof(player), 0);
	//		if (retval == SOCKET_ERROR) {
	//			err_display((char*)"recv()");
	//			exit(1);
	//		}

	//		if (player.loginState == 1) {
	//			if (player.isAlreadyExist)
	//				STD cout << STD endl << "�̹� �����ϴ� ���̵��Դϴ�." << STD endl << STD endl;
	//			else
	//				STD cout << STD endl << "���̵� �����߽��ϴ�." << STD endl << STD endl;
	//		}

	//		if (player.loginState == 2 && player.sceneNumber == 0)
	//		{
	//			if (player.isAlreadyLoggedIn) {
	//				STD cout << STD endl << "�̹� ������ ���̵� �Դϴ�." << STD endl << STD endl;
	//				player.isAlreadyLoggedIn = false;
	//			}
	//			else
	//				STD cout << STD endl << "���� ������ ����ġ�մϴ�." << STD endl << STD endl;
	//		}
	//	}

	//	else if (player.sceneNumber == LOBBYSCENE)
	//	{
	//		if (gameStart)
	//			return;

	//		system("cls");
	//		STD cout << "�κ� �����߽��ϴ�." << STD endl;

	//		retval = recvn(clientSock, (char *)&player, sizeof(player), 0);
	//		if (retval == SOCKET_ERROR) {
	//			break;
	//		}
	//	}

	//	else if (player.sceneNumber == INGAMESCENE) {
	//		STD cout << "���ӿ� ���� �߽��ϴ�." << STD endl;
	//		gameStart = true;
	//		break;
	//	}
	//}
	//ClientNum = player.actorNumber;
	//STD cout << "�� ��ȣ�� : " << ClientNum << " �Դϴ�." << STD endl;
	//#pragma endregion
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

void Network::ReceiveRenderData()
{
	while (1)
	{
		retval = recvn(Nw.clientSock, (char*)&vecSize, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char*>("recv()"));
			exit(0);
		}

		rendererData.resize(vecSize);
		retval = recvn(clientSock, (char*)&rendererData[0], sizeof(RenderData) * vecSize, 0);
		if (retval == SOCKET_ERROR) {
			err_display(const_cast<char*>("recv()"));
			exit(0);
		}
	}
}
