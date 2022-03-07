#pragma once;
#include "Game.h"

void Printc(int space, char c, string str)
{
	int l = (space - str.size()) / 2;
	int r = (space - str.size() + 1) / 2;
	while ((l--) > 0)
		cout << c;
	cout << str;
	while ((r--) > 0)
		cout << c;
}

short Parse(string str, short failure)
{
	short result = 0;
	int lim = min(str.length(), 4);
	for (int i = 0; i < lim; i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return failure;
		result = 10 * result + str[i] - '0';
	}
	return result;
}

Condition::Condition(unsigned short seed)
{
	srand(seed);
	_type = rand() % 5;
	switch (_type)
	{
	case 0:
		_dmg = rand() % 25 + 5;
		_paramsCount = 3;
		_params = new long[3];
		_params[0] = rand() % 9 + 1;
		_params[1] = rand() % 9 + 1;
		_params[2] = rand() % 25000 + 10000;
	case 1:
		_dmg = rand() % 20 + 5;
		_paramsCount = 3;
		_params = new long[3];
		_params[0] = rand() % 9 + 1;
		_params[1] = rand() % 9 + 1;
		_params[2] = rand() % 9000 + 1000;
		break;
	case 2:
		_dmg = rand() % 30 + 15;
		_paramsCount = 1;
		_params = new long[1];
		_params[0] = (rand() % 1700)*(rand() % 1700) + 101000;
		break;
	case 3:
		_dmg = rand() % 40 + 1;
		_paramsCount = 1;
		_params = new long[1];
		_params[0] = (rand() % 3000)*(rand() % 3000) + 7000000;
		break;
	case 4:
		_dmg = rand() % 15 + 5;
		_paramsCount = 1;
		_params = new long[1];
		_params[0] = rand() % 15 + 3;
		break;
	}
}

string Condition::GetText()
{
	switch (_type)
	{
	case 0:
		return to_string(_params[0]) + "a+" + to_string(_params[1]) + "b>" + to_string(_params[2]);
	case 1:
		return "|" + to_string(_params[0]) + "a-" + to_string(_params[1]) + "b|<" + to_string(_params[2]);
	case 2:
		return "a*b<" + to_string(_params[0]);
	case 3:
		return "a^2+b^2>" + to_string(_params[0]);
	case 4:
		return "a//b>" + to_string(_params[0]);
	}
	return "";
}

long Condition::GetDamage()
{
	return _dmg;
}

bool Condition::Check(short a, short b)
{
	switch (_type)
	{
	case 0:
		return _params[0] * (long)a + _params[1] * (long)b > _params[2];
	case 1:
		return abs(_params[0] * (long)a - _params[1] * (long)b) < _params[2];
	case 2:
		return (long)a * (long)b < _params[0];
	case 3:
		return a * (long)a + b * (long)b > _params[0];
	case 4:
		return a / b > _params[0];
	}
	return false;
}

Condition::~Condition()
{
	delete[] _params;
}

Condition::Condition(const Condition& obj)
{
	_type = obj._type;
	_paramsCount = obj._paramsCount;
	_params = new long[_paramsCount];
	memcpy(_params, obj._params, _paramsCount * sizeof(long));
	_dmg = obj._dmg;
}

Player::Player(const char *name, long hp)
{
	this->Name = name;
	this->Hp = hp;
}

Room::Room(const char *name, long maxhp, const char *p1Name, const char *p2Name) : _p1(p1Name, maxhp), _p2(p2Name, maxhp)
{
	this->_name = name;
	this->_maxHp = maxhp;
}

void Room::Draw()
{
	Printc(60, '-', _name);
	cout << endl;
	Printc(30, ' ', _p1.Name);
	Printc(30, ' ', _p2.Name);
	cout << endl;
	Printc(30, ' ', "hp: " + to_string(_p1.Hp) + "/" + to_string(_maxHp));
	Printc(30, ' ', "hp: " + to_string(_p2.Hp) + "/" + to_string(_maxHp));
	cout << endl;
	for (int i = 0; i < _p1.Conditions.size(); i++)
	{
		Printc(20, ' ', _p1.Conditions[i].GetText());
		cout << '|';
		Printc(9, ' ', "-" + to_string(_p1.Conditions[i].GetDamage()));
		Printc(20, ' ', _p2.Conditions[i].GetText());
		cout << '|';
		Printc(9, ' ', "-" + to_string(_p2.Conditions[i].GetDamage()));
		cout << endl;
	}
}

int Room::Update(short a, short b, unsigned short seed1, unsigned short seed2)
{
	for (auto condition : _p1.Conditions)
		if (!condition.Check(a, b))
			_p1.Hp -= condition.GetDamage();
	for (auto condition : _p2.Conditions)
		if (!condition.Check(a, b))
			_p2.Hp -= condition.GetDamage();
	_p1.Conditions.push_back(Condition(seed1));
	_p2.Conditions.push_back(Condition(seed2));
	if (_p1.Hp <= 0 && _p2.Hp <= 0)
		return 3;
	else if (_p1.Hp <= 0)
		return 2;
	else if (_p2.Hp <= 0)
		return 1;
	return 0;
}