#include "Package.h"

Package::Package()
{
	Data = new uint8[PACKAGE_SIZE];
	Pos = 0;
}

Package::~Package()
{
	delete[] Data;
}

void Package::Add8(uint8 val)
{
	*(Data + Pos) = val;
	Pos++;
}

void Package::Add16(uint16 val)
{
	*(Data + Pos) = val & 0x00ff;
	*(Data + Pos + 1) = (val & 0xff00) >> 8;
	Pos += 2;
}

void Package::Add32(uint32 val)
{
	*(Data + Pos) = val & 0x000000ff;
	*(Data + Pos + 1) = (val & 0x0000ff00) >> 8;
	*(Data + Pos + 2) = (val & 0x00ff0000) >> 16;
	*(Data + Pos + 3) = (val & 0xff000000) >> 24;
	Pos += 4;
}

uint8 Package::Read8()
{
	uint8 b1 = *(Data + (Pos++));
	return b1;
}

uint16 Package::Read16()
{
	uint16 b1 = *(Data + (Pos++));
	uint16 b2 = *(Data + (Pos++)) << 8;
	return b1 + b2;
}

uint32 Package::Read32()
{
	uint32 b1 = *(Data + (Pos++));
	uint32 b2 = *(Data + (Pos++)) << 8;
	uint32 b3 = *(Data + (Pos++)) << 16;
	uint32 b4 = *(Data + (Pos++)) << 24;
	return b1 + b2 + b3 + b4;
}

void Package::AddStr(const char *str, int len)
{
	Add32(len);
	for (int i = 0; i < len; i++)
		Add8(str[i]);
}

char* Package::ReadStr()
{
	int len = Read32();
	char *str = new char[len];
	for (int i = 0; i < len; i++)
		*(str + i) = Read8();
	return str;
}

void Package::Reset()
{
	Pos = 0;
}
