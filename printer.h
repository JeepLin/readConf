// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.
//
// Utility class for writing text to a ZeroCopyOutputStream.

#ifndef _PRINTER_H__
#define _PRINTER_H__

#include <string>
#include <string.h>
#include <map>
#include "stream.h"

using namespace std;

//   Printer printer(output, '$');
//   map<string, string> vars;
//   vars["name"] = "Bob";
//   printer.Print(vars, "My name is $name$.");
//
// The above writes "My name is Bob." to the output stream.
//
// Printer aggressively enforces correct usage, crashing (with assert failures)
// in the case of undefined variables in debug builds. This helps greatly in
// debugging code which uses it.
//class LIBPROTOBUF_EXPORT Printer {
class Printer {
 public:
  // Create a printer that writes text to the given output stream.  Use the
  // given character as the delimiter for variables.

//  Printer(ZeroCopyOutputStream* output, char variable_delimiter);
  Printer(ZeroCopyOutputStream* output, char variable_delimiter);
  ~Printer();

  // Print some text after applying variable substitutions.  If a particular
  // variable in the text is not defined, this will crash.  Variables to be
  // substituted are identified by their names surrounded by delimiter
  // characters (as given to the constructor).  The variable bindings are
  // defined by the given map.
  void Print(const map<string, string>& variables, const char* text);

  // Like the first Print(), except the substitutions are given as parameters.
  void Print(const char* text);
  // Like the first Print(), except the substitutions are given as parameters.
  void Print(const char* text, const char* variable, const string& value);
  // Like the first Print(), except the substitutions are given as parameters.
  void Print(const char* text, const char* variable1, const string& value1,
                               const char* variable2, const string& value2);
  // Like the first Print(), except the substitutions are given as parameters.
  void Print(const char* text, const char* variable1, const string& value1,
                               const char* variable2, const string& value2,
                               const char* variable3, const string& value3);
  // TODO(kenton):  Overloaded versions with more variables?  Three seems
  //   to be enough.

  // Indent text by two spaces.  After calling Indent(), two spaces will be
  // inserted at the beginning of each line of text.  Indent() may be called
  // multiple times to produce deeper indents.
  void Indent();

  // Reduces the current indent level by two spaces, or crashes if the indent
  // level is zero.
  void Outdent();

  // Write a string to the output buffer.
  // This method does not look for newlines to add indentation.
  void PrintRaw(const string& data);

  // Write a zero-delimited string to output buffer.
  // This method does not look for newlines to add indentation.
  void PrintRaw(const char* data);

  // Write some bytes to the output buffer.
  // This method does not look for newlines to add indentation.
  void WriteRaw(const char* data, int size);

  // True if any write to the underlying stream failed.  (We don't just
  // crash in this case because this is an I/O failure, not a programming
  // error.)
  bool failed() const { return failed_; }

 private:
  const char variable_delimiter_;
  ZeroCopyOutputStream* const output_;
//  CopyingOutputStreamAdaptor* const output_;
  char* buffer_;
  int buffer_size_;
  string indent_;
  bool at_start_of_line_;
  bool failed_;
//  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Printer);
};

#endif  // GOOGLE_PROTOBUF_IO_PRINTER_H__
