// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#ifndef _CPP_FILE_H__
#define _CPP_FILE_H__

#include <string>
#include <vector>
#include <map>
#include "common.h"
#include "printer.h"

using namespace std;

class FileDescriptor{
public:
	/*
	map<string,string> argStr;
	map<string,int> argInt;
	map<string,double> argDouble;
	*/

	map<string,string> defArgStr;
	map<string,int> defArgInt;
	map<string,double> defArgDouble;

	vector<pair<string,int> > defArg; 
	void print();
	void MessageGenerator(int argc,char **argv);
	int whitchType(char *argv);
	string name();
	string name_;
};

class FileGenerator {
public:
  FileGenerator(FileDescriptor* file):file_(file){};
  ~FileGenerator(){};

  void GenerateHeader(Printer* printer);
  void GenerateSource(Printer* printer);

private:
  FileDescriptor* file_;
};


#endif  // GOOGLE_PROTOBUF_COMPILER_CPP_FILE_H__
