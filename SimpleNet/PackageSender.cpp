#include "PackageSender.h"

#pragma warning(disable: 4996)

PackageSender::PackageSender(const char * addr, uint16 port)
{
	_info.sin_family = AF_INET;
	_info.sin_port = htons(port);
	_info.sin_addr.s_addr = inet_addr(addr);
	_infoSize = sizeof(_info);

	_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

PackageSender::~PackageSender()
{
	WSACleanup();
	closesocket(_socket);
}

void PackageSender::Send(Package &msg, Package &respond)
{
	respond.Reset();
	sendto(_socket, (char*)msg.Data, msg.Pos, 0, (SOCKADDR*)&_info, _infoSize);
	recvfrom(_socket, (char*)respond.Data, PACKAGE_SIZE, 0, (SOCKADDR*)&_info, &_infoSize);
}