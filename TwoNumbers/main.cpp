#include "CommandLine.h"

void Help(const CommandLine& console, const vector<string>& args)
{
	cout << "help, set, test" << endl;
}

void Set(CommandLine& console, const vector<string>& args)
{
	console.SetVariable("@" + args[1], args[2]);
}

void Test(const CommandLine& console, const vector<string>& args)
{
	cout << args[1] << endl;
}

int main()
{
	CommandLine cmd;
	cmd.AddCommand(Command("help", Help, 0));
	cmd.AddCommand(Command("set", Set, 2));
	cmd.AddCommand(Command("test", Test, 1));
	cmd.Run();
}