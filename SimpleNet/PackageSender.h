#pragma once
#include "Package.h"
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

class PackageSender
{
private:
	SOCKET _socket;
	sockaddr_in _info;
	int _infoSize;
public:
	PackageSender(const char *addr, uint16 port);
	~PackageSender();
	void Send(Package &msg, Package &respond);
};