#ifndef _COMMAND_LINE_INTERFACE_H__
#define _COMMAND_LINE_INTERFACE_H__
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class CommandInterface
{
public:
	CommandInterface(){}
	~CommandInterface(){}
	
	bool parseArguments(int argc,char **argv);
	void Run();

	vector<string> files;
};

#endif
