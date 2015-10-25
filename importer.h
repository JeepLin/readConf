#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "sds.h"
#include "cpp_file.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>

using namespace std;

class Importer
{
public:
	void loadServerConfigFromString(char *,FileDescriptor *);	
	FileDescriptor *loadServerConfig(const char *filename);
};

#endif
