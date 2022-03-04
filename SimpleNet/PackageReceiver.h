#pragma once
#include "Package.h"
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

class PackageReceiver
{
private:
	SOCKET _socket;
	sockaddr_in _info;
	int _infoSize;
public:
	PackageReceiver(const char *addr, uint16 port);
	~PackageReceiver();
	void Receive(Package &result);
	void Respond(Package &msg);
};

