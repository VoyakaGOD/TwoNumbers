#include "SimpleNet.h"
#include "CommandLine.h"
#include "Game.h"

#define GAME_PORT 26999
#define GAME_MAGIC (uint32)8785327
#define GAME_MAGIC2 (uint32)3780795

PackageSender *sender = NULL;
PackageReceiver *receiver = NULL;
string name = "Player";

void ShowHelp(const CommandLine& console, const vector<string>& args)
{
	cout << "-------------------------------" << endl;
	cout << "help - shows a list of commands" << endl;
	cout << "rules - shows info about game" << endl;
	cout << "set [name] [value] - sets variable" << endl;
	cout << "name [name] - changes the player's name" << endl;
	cout << "create [name] [maxHp] - create room" << endl;
	cout << "connect [ip] - find room by ip and enter" << endl;
	cout << "-------------------------------" << endl;
}

void ShowRules(const CommandLine& console, const vector<string>& args)
{
	cout << "The rules of the game are very simple:" << endl;
	cout << "each player has conditions written under his nickname" << endl;
	cout << "each player enters a number between 0 and 9999" << endl;
	cout << "Then the conditions are checked and for each condition that is performed, the player receives damage said to the right of the condition";
	cout << "also after each turn each player get the new condition" << endl;
}

void Set(CommandLine& console, const vector<string>& args)
{
	console.SetVariable("@" + args[1], args[2]);
}

void ChangeName(const CommandLine& console, const vector<string>& args)
{
	if (args[1].length() > MAX_NAME_LEN)
		name = args[1].substr(0, MAX_NAME_LEN);
	name = args[1];
}

void StartGame(bool isHost, const char* roomName, long MHP, const char* p1, const char* p2)
{
	Package package;
	Package respond;
	Room room(roomName, MHP, p1, p2);
	room.Draw();
	cout << "(s)>>>";
	unsigned short s = time(NULL);
	int winner;
	while (true)
	{
		srand(s);
		s = rand();
		string input;
		getline(cin, input);

		short a = 0;
		short b = 0;
		uint16 s1 = 0;
		uint16 s2 = 0;

		if (isHost)
		{
			a = Parse(input, rand() % MAX_NUMBER);
			receiver->Receive(package);
			if (package.Read32() == GAME_MAGIC2)
			{
				b = package.Read16();
				s1 = rand();
				s2 = rand();
				package.Reset();
				package.Add32(GAME_MAGIC2);
				package.Add16(a);
				package.Add16(s1);
				package.Add16(s2);
				receiver->Respond(package);
			}
			else
			{
				cout << "Strange error" << endl;
				return;
			}
		}
		else
		{
			b = Parse(input, rand() % MAX_NUMBER);
			package.Reset();
			package.Add32(GAME_MAGIC2);
			package.Add16(b);
			sender->Send(package, respond);
			if (respond.Read32() == GAME_MAGIC2)
			{
				a = respond.Read16();
				s1 = respond.Read16();
				s2 = respond.Read16();
			}
			else
			{
				cout << "Strange error" << endl;
				return;
			}
		}

		cout << "(a:" << a << "; b:" << b << ")" << endl;
		winner = room.Update(a, b, s1, s2);
		getline(cin, input);
		system("cls");
		if (winner != 0)
			break;
		room.Draw();
		cout << "(" << (isHost ? 'a' : 'b') << ")>>>";
	}
	switch (winner)
	{
	case 1:
		cout << p1 << " won!" << endl;
		break;
	case 2:
		cout << p2 << " won!" << endl;
		break;
	case 3:
		cout << "DRAW!" << endl;
		break;
	}
}

void Create(const CommandLine& console, const vector<string>& args)
{
	Package package;
	StartupSimpleNet();
	char *ip = GetIP();
	cout << ip << endl;
	receiver = new PackageReceiver(ip, GAME_PORT);
	cout << "Waiting for an opponent." << endl;
	receiver->Receive(package);
	if(package.Read32() == GAME_MAGIC)
	{
		char *p2 = package.ReadStr();
		package.Reset();
		package.Add32(GAME_MAGIC);
		package.AddStr(args[1].c_str(), args[1].length() + 1);
		package.Add32(atol(args[2].c_str()));
		package.AddStr(name.c_str(), name.length() + 1);
		receiver->Respond(package);
		StartGame(true, args[1].c_str(), atol(args[2].c_str()), name.c_str(), p2);
		delete[] p2;
	}
	else
	{
		cout << "Strange error" << endl;
	}
	delete receiver;
}

void Connect(const CommandLine& console, const vector<string>& args)
{
	Package package;
	Package respond;
	StartupSimpleNet();
	sender = new PackageSender(args[1].c_str(), GAME_PORT);
	package.Add32(GAME_MAGIC);
	package.AddStr(name.c_str(), name.length() + 1);
	sender->Send(package, respond);
	if (respond.Read32() == GAME_MAGIC)
	{
		char *rname = respond.ReadStr();
		long MHP = respond.Read32();
		char *p2 = respond.ReadStr();
		StartGame(false, rname, MHP, p2, name.c_str());
		delete[] rname;
		delete[] p2;
	}
	else
	{
		cout << "Strange error" << endl;
	}
	delete sender;
}

int main()
{
	cout << "<for basic information enter help>" << endl;

	CommandLine cmd;
	cmd.AddCommand(Command("help", ShowHelp, 0));
	cmd.AddCommand(Command("rules", ShowRules, 0));
	cmd.AddCommand(Command("set", Set, 2));
	cmd.AddCommand(Command("name", ChangeName, 1));
	cmd.AddCommand(Command("create", Create, 2));
	cmd.AddCommand(Command("connect", Connect, 1));
	cmd.Run();
}