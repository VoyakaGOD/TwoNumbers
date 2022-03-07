#include "PackageReceiver.h"

#pragma warning(disable: 4996)

PackageReceiver::PackageReceiver(const char * addr, uint16 port)
{
	_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	_info.sin_family = AF_INET;
	_info.sin_port = htons(port);
	_info.sin_addr.s_addr = inet_addr(addr);
	_infoSize = sizeof(_info);

	bind(_socket, (SOCKADDR*)&_info, _infoSize);
}

PackageReceiver::~PackageReceiver()
{
	WSACleanup();
	closesocket(_socket);
}

void PackageReceiver::Receive(Package &result)
{
	result.Reset();
	recvfrom(_socket, (char*)result.Data, PACKAGE_SIZE, 0, (SOCKADDR*)&_info, &_infoSize);
}

void PackageReceiver::Respond(Package &msg)
{
	sendto(_socket, (char*)msg.Data, msg.Pos, 0, (SOCKADDR*)&_info, _infoSize);
}