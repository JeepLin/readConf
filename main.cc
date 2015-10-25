#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iterator>
#include "command_interface.h"
#include "common.h"

int main(int argc, char **argv) 
{
	CommandInterface cli;

	JEEP_FETAL_CHECK(cli.parseArguments(argc,argv),"main ");

	cli.Run();
	
	return 0;
}
