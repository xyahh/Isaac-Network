#pragma once
#pragma pack(1)
struct Pos {
	int x;
	int y;
};
#pragma pack()


struct KeyData {
	int key = 0;
	int clientNum = 0;
	bool pressed = false;
};


struct RenderData2 {
	DX XMFLOAT2 Current;
	DX XMFLOAT2 Total;
	DX XMFLOAT3 SpriteOffset;
	DX XMFLOAT2 SpriteSize;
	size_t TextureID = -1;
};

struct RenderData {
	DX XMFLOAT3 Position;
	DX XMFLOAT4 Color;
	RenderData2 RenderDataSet[3];
};

class Network
{
public:
	Network();
	~Network();

	static Network GameNetwork;

	void err_quit(char *msg);
	void err_display(char *msg);

	void AcceptClients();
	void SendRenderData();
//	DWORD WINAPI ServerMain(LPVOID p);
	void testFunc();
	STD vector<DX XMVECTOR> Positions;
	STD vector <RenderData> rendererData;
	STD queue<KeyData> InputQueue;
	//STD vector<STD queue<KeyData>> InputQueues;
	int recvn(SOCKET s, char * buf, int len, int flags);
private:
	int CurrentClientNum = 0;
	int EnteredClientsNum = 0;
	STD vector<SOCKET> ClientSockets;

	 int retval = 0;
	 SOCKET listen_sock;
};



extern Network Nw;