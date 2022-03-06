#include "CommandLine.h"

#include "SimpleNet.h"
#include "Game.h"

PackageSender sender("127.0.0.1", 26999);

void Help(const CommandLine& console, const vector<string>& args)
{
	cout << "help, set, test" << endl;
}

void Set(CommandLine& console, const vector<string>& args)
{
	console.SetVariable("@" + args[1], args[2]);
}

void Create(const CommandLine& console, const vector<string>& args)
{
	Package package;
	Package respond;
	package.Add8(0);
	package.AddStr(args[1].c_str(), args[1].length() + 1);
	sender.Send(package, respond);
	cout << respond.ReadStr() << endl;
}

void Connect(const CommandLine& console, const vector<string>& args)
{
	cout << args[1] << endl;
}

void DisplayList(const CommandLine& console, const vector<string>& args)
{
	Package package;
	package.Add8(1);
	Package respond;
	sender.Send(package, respond);
	uint8 count = respond.Read8();
	for (int i = 0; i < count; i++)
		cout << respond.ReadStr() << endl;
}

void Test(const CommandLine& console, const vector<string>& args)
{
	Room tr("<Test room>", 200, "Wolf", "Fox");
	tr.Draw();
	cout << "(s)>>>";
	unsigned short s = time(NULL);
	int winner;
	while (true)
	{
		srand(s);
		s = rand();
		string input;
		getline(cin, input);
		short a = Parse(input, rand() % MAX_NUMBER);
		short b = rand() % MAX_NUMBER;
		cout << "(a:" << a << "; b:" << b << ")" << endl;
		winner = tr.Update(a, b, rand(), rand());
		getline(cin, input);
		system("cls");
		if (winner != 0)
			break;
		tr.Draw();
		cout << "(a)>>>";
	}
	if (winner == 3)
		cout << "DRAW!" << endl;
	else
		cout << "P" << winner << " won!" << endl;
}

int main()
{
	CommandLine cmd;
	cmd.AddCommand(Command("help", Help, 0));
	cmd.AddCommand(Command("set", Set, 2));
	/*cmd.AddCommand(Command("create", Create, 2));
	cmd.AddCommand(Command("name", Connect, 1));
	cmd.AddCommand(Command("server", Connect, 1));*/
	cmd.AddCommand(Command("test", Test, 0));
	cmd.Run();
}