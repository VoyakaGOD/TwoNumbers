#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

WSADATA _wsa;

void StartupSimpleNet()
{
	WSAStartup(MAKEWORD(2, 2), &_wsa);
}

char* GetIP()
{
	hostent *h = gethostbyname(NULL);
	in_addr addr;
	addr.s_addr = *(u_long *)h->h_addr_list[0];
	return inet_ntoa(addr);
}