#pragma once
#include "Renderer.h"
#pragma pack(1)
struct Pos {
   int x;
   char c;
   int y;
};
#pragma pack()



struct KeyData {
	int key;
	int clientNum;
	bool pressed;
};

class Network
{
public:
   Network();
   ~Network();

   void err_quit(char *msg);
	void err_display(char *msg);
	//static DWORD WINAPI ProcessClient(LPVOID arg);
	//void recvstart();
	//void testFunc(SOCKET sock);a
	void sendInput(KeyData k);
	STD vector<DX XMVECTOR> Positions;

	void Init();

	Renderer RenderDevice;
	int TEX;
	SOCKET clientSock;
	int retval;
	int recvn(SOCKET s, char * buf, int len, int flags);

};

struct Arg {
	Network* p;
	SOCKET sock;
};

extern Network NW;