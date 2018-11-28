
#pragma pack(1)
struct Pos {
   int x;
   int y;
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
	void start();
	void testFunc(SOCKET sock);

private:
	SOCKET clientSock;

   int retval;
   Pos v[3] = { 0, };

};

struct Arg {
	Network* p;
	SOCKET sock;
};
