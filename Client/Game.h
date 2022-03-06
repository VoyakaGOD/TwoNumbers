#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <functional>
#include <vector>
#include <time.h>

using namespace std;

#define MAX_NUMBER (short)10000
#define MAX_NAME_LEN 20

void Printc(int space, char c, string str);

short Parse(string str, short failure);

class Condition
{
private:
	short _type;
	int _paramsCount;
	long* _params;
	long _dmg;
public:
	Condition(unsigned short seed);
	string GetText();
	long GetDamage();
	bool Check(short a, short b);
	~Condition();
	Condition(const Condition& obj);
};

struct Player
{
	const char* Name;
	long Hp;
	vector<Condition> Conditions;

	Player(const char *name, long hp);
};

class Room
{
private:
	const char* _name;
	long _maxHp;
	Player _p1;
	Player _p2;
public:
	Room(const char *name, long maxhp, const char *p1Name, const char *p2Name);
	void Draw();
	int Update(short a, short b, unsigned short seed1, unsigned short seed2);
};