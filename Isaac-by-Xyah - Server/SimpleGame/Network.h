#pragma once
#pragma pack(1)
struct Pos {
	int x;
	int y;
};
#pragma pack()


struct KeyData {
	int key;
	bool pressed;
	int clientNum;
};


class Network
{
public:
	Network();
	~Network();

	void err_quit(char *msg);
	void err_display(char *msg);

	void acceptClients();
	static DWORD WINAPI ServerMain(LPVOID p);
	void testFunc(int num);

	STD queue<KeyData> InputQueue;
private:
	int clientNum = 0;

	 int retval = 0;
	 Pos v[3] = { 0, };
	 SOCKET listen_sock;
	 STD vector<SOCKET> ClientSockets;

};


struct Arg {
	Network* p;
	int clientnum;
};
