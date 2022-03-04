#include "CommandLine.h"

#include "SimpleNet.h"

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
	auto package = Package();
	auto respond = Package();
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
	auto package = Package();
	package.Add8(1);
	auto respond = Package();
	sender.Send(package, respond);
	uint8 count = respond.Read8();
	for (int i = 0; i < count; i++)
		cout << respond.ReadStr() << endl;
}

int main(int argc, char **argv)
{
	CommandLine cmd;
	cmd.AddCommand(Command("help", Help, 0));
	cmd.AddCommand(Command("set", Set, 2));
	cmd.AddCommand(Command("create", Create, 2));
	cmd.AddCommand(Command("connect", Connect, 1));
	cmd.AddCommand(Command("list", DisplayList, 0));
	cmd.Run();
}