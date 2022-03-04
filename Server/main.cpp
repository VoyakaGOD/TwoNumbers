#include <iostream>
#include <string>
#include <vector>

#include "SimpleNet.h"

using namespace std;

int main()
{
	vector<char*> strs;
	StartupSimpleNet();
	char *ip = GetIP();
	cout << "addr: " << ip << endl;
	PackageReceiver receiver("127.0.0.1", 26999);
	auto package = Package();
	while (true)
	{
		package.Reset();
		receiver.Receive(package);
		uint8 type = package.Read8();
		if (type == 0)
		{
			strs.push_back(package.ReadStr());
			package.Reset();
			package.AddStr("all is good", 12);
		}
		else
		{
			package.Reset();
			uint8 count = (strs.size() < 10) ? strs.size() : 10;
			package.Add8(count);
			for (int i = 0; i < count; i++)
				package.AddStr(strs[i], strlen(strs[i]) + 1);
		}
		cout << "type" << (int)type << endl;
		receiver.Respond(package);
	}
}