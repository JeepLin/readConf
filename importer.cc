#include "importer.h"
#include "sds.h"
#include "common.h"
#include <iostream>
using namespace std;

void Importer::loadServerConfigFromString(char *config,FileDescriptor *vars)
{
		string err;
		int linenum = 0, totlines, i, j;
		int slaveof_linenum = 0;
		sds *lines;

		lines = sdssplitlen(config,strlen(config),"\n",1,&totlines);
		
		for (i = 0; i < totlines; i++) 
		{
				sds *argv;
				int argc;

				linenum = i+1;
				lines[i] = sdstrim(lines[i]," \t\r\n");

				if (lines[i][0] == '#' || lines[i][0] == '\0') continue;

				argv = sdssplitargs(lines[i],&argc);
				if (argv == NULL) {
						err = "Unbalanced quotes in configuration line";
						goto loaderr;
				}

				if (argc == 0) {
						sdsfreesplitres(argv,argc);
						continue;
				}
				sdstolower(argv[0]);
				
				if(argc== 2 || argc== 3)
				{
					vars->MessageGenerator(argc,argv);
				}
				else
				{
					continue;
				}
		}

		sdsfreesplitres(lines,totlines);
		return;

loaderr:
		fprintf(stderr, "\n*** FATAL CONFIG FILE ERROR ***\n");
		fprintf(stderr, "Reading the configuration file, at line %d\n", linenum);
		fprintf(stderr, ">>> '%s'\n", lines[i]);
		fprintf(stderr, "%s\n", err.c_str());
		exit(1);
}


FileDescriptor *Importer::loadServerConfig(const char *filename)
{
		sds config = sdsempty();
		char buf[1024];

		/* Load the file content */
		if (filename) 
		{
			FILE *fp;

			if ((fp = fopen(filename,"r")) == NULL) 
			{
					exit(1);
			}

			while(fgets(buf,1024+1,fp) != NULL)
			{
				config = sdscat(config,buf);
//				printf("buf:%s\n",buf);
			}
			fclose(fp);
		}
		/* Append the additional options */
		/*
		if (options) {
				config = sdscat(config,"\n");
				config = sdscat(config,options);
		}
		*/
		string name= filename;
		FileDescriptor *vars= new FileDescriptor();
		int pos= name.find('.',0);
		vars->name_= name.substr(0,(pos== string::npos)?name.size():pos)+ "Init";

		cout<<"filename:"<< vars->name()<< endl;
		loadServerConfigFromString(config,vars);
		sdsfree(config);

		return vars;
}
