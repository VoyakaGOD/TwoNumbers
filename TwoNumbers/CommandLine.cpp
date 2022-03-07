#include "CommandLine.h"

CommandLineVariable::CommandLineVariable(const string& name, const string& value)
{
	Name = name;
	Value = value;
}

Command::Command(const char* name, CmdFunc function, int argsCount)
{
	Name = name;
	Function = function;
	ArgsCount = argsCount;
}

string* CommandLine::GetVariableValue(const string& name)
{
	int index = 0;
	for (auto var : _variables)
	{
		if (var.Name == name)
			break;
		index++;
	}
	if (index >= _variables.size())
		return NULL;
	return &_variables.at(index).Value;
}

void CommandLine::SetVariable(const string& name, const string& value)
{
	string* val = GetVariableValue(name);
	if (val == NULL)
		_variables.push_back(CommandLineVariable(name, value));
	else
		*val = value;
}

void CommandLine::AddCommand(const Command& cmd)
{
	_commands.push_back(cmd);
}

vector<string> Split(const string& str, char sep)
{
	vector<string> parts;
	int pointer = 0;
	int len = 0;

	while (char c = str[pointer])
	{
		if (c == ' ')
		{
			if (len > 0)
			{
				parts.push_back(str.substr(pointer - len, len));
				len = 0;
			}
		}
		else
		{
			len++;
		}
		pointer++;
	}
	if(len > 0)
		parts.push_back(str.substr(pointer - len, len));
	return parts;
}

void CommandLine::Run()
{
	string input;
	while (true)
	{
		getline(cin, input);
		auto words = Split(input, ' ');
		if (words.size() == 0)
			continue;
		for (auto& w : words) 
		{
			if (w[0] == '@')
			{
				string* val = GetVariableValue(w);
				w = (val == NULL)? "null" : *val;
			}
		}

		bool found = false;
		for (auto cmd : _commands)
		{
			if (cmd.Name == words[0] && words.size() == (cmd.ArgsCount + 1))
			{
				cmd.Function(*this, words);
				found = true;
				break;
			}
		}
		if (!found)
			cout << "function not found" << endl;
	}
}