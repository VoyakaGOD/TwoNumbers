#pragma once
#include<vector>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

class CommandLine;

typedef function<void(CommandLine& console, const vector<string>& args)> CmdFunc;

struct CommandLineVariable
{
	string Name;
	string Value;
	CommandLineVariable(const string& name, const string& value);
};

struct Command
{
	const char* Name;
	CmdFunc Function;
	int ArgsCount;
	Command(const char* name, CmdFunc function, int argsCount);
};

class CommandLine
{
private:
	vector<CommandLineVariable> _variables;
	vector<Command> _commands;
	string* GetVariableValue(const string& name);
public:
	void SetVariable(const string& name, const string& value);
	void AddCommand(const Command& cmd);
	void Run();
};