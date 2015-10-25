#include <fcntl.h>
#include "sds.h"
#include "importer.h"
#include "printer.h"
#include "stream.h"
#include "cpp_file.h"
#include "command_interface.h"

bool CommandInterface::parseArguments(int argc,char **argv)
{
	bool bRet= false;

	for(int i= 1;i< argc;i++)
	{
		cout<< "argv:"<< argv[i]<< endl;
			
		if (argv[i]!= NULL && argv[i][0] != '-' && argv[i][1] != '-')
		{
			files.push_back(argv[i]);
		}
	}

	if(files.size()> 0)
		bRet= true;
	
	return bRet;
}


void CommandInterface::Run()
{
	Importer importer;
	for(int i= 0;i< files.size();i++)
	{
		FileDescriptor *fdes= importer.loadServerConfig(files[i].c_str());
		JEEP_ERROR_CHECK((fdes!=NULL),"main ");

		int fdh= open((fdes->name()+ ".h").c_str(),O_RDWR| O_CREAT);
		JEEP_ERROR_CHECK(fdh!= 0, "main ");
		
		int fds= open((fdes->name()+ ".cpp").c_str(),O_RDWR| O_CREAT);
		JEEP_ERROR_CHECK(fds!= 0, "main ");
		
		//string
		//string tmp;
		//StringOutputStream test(&tmp);
		//std::cout<< tmp.c_str()<< std::endl;
		//fdes->print();
		
		//file
		FileOutputStream* pcosah= new FileOutputStream(fdh,1024);
		Printer* printerH= new Printer(pcosah,'$');
		
		FileOutputStream* pcosas= new FileOutputStream(fds,1024);
		Printer* printerS= new Printer(pcosas,'$');

		FileGenerator fileCreater(fdes);
		fileCreater.GenerateHeader(printerH);
		fileCreater.GenerateSource(printerS);

		delete printerS;
		delete printerH;
		delete pcosah;
		delete pcosas;
		delete fdes;
	}
}
