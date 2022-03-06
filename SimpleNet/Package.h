#pragma once

#define PACKAGE_SIZE 256

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

struct Package
{
	uint8* Data;
	int Pos;

	Package();
	~Package();
	void Add8(uint8 val);
	void Add16(uint16 val);
	void Add32(uint32 val);
	uint8 Read8();
	uint16 Read16();
	uint32 Read32();
	void AddStr(const char *str, int len);
	char* ReadStr();
	void Reset();

private:
	Package(const Package& obj);
	void operator=(const Package& obj);
};