
#pragma pack(1)
struct Pos {
   int x;
   char c;
   int y;
};
#pragma pack()



#pragma pack(1)
struct KeyData {
	unsigned char key;
	bool pressed;
	int clientNum;
};
#pragma pack()

class Network
{
public:
   Network();
   ~Network();

   void err_quit(char *msg);
	void err_display(char *msg);
	static DWORD WINAPI ProcessClient(LPVOID arg);
	void recvstart();
	void testFunc(SOCKET sock);
	void sendInput(KeyData k);
private:
	SOCKET clientSock;

   int retval;
   Pos v[3] = { 0, };

};

struct Arg {
	Network* p;
	SOCKET sock;
};

extern Network NW;