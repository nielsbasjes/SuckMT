//=========================================================================
//                 Copyright (C)1999-2003 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT/
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : MultiStream.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2003/04/13 20:51:55 $
//  $Revision: 1.6 $
//  $RCSfile: MultiStream.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   If you reuse code from SuckMT you are required to put a notice of 
//   this fact in both your manual and about box.
//
//=========================================================================

#ifndef __MULTISTREAM_H__
#define __MULTISTREAM_H__

//-------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <map>

//-------------------------------------------------------------------------

using namespace std;

//-------------------------------------------------------------------------
// Generic defines
#define BIT(b)  (long(long(1)<<long(b)))

// Max length of a log message
#define MAX_LOGGER_SIZE 1000

//-------------------------------------------------------------------------
// Set of possible log levels you can specify for a LogStream
#define LOGS_ALWAYS      0
#define LOGS_INFO        1
#define LOGS_STATUS      2
#define LOGS_VERBOSE     3
#define LOGS_COMMANDS    4
#define LOGS_CALLS       5
#define LOGS_NOISE       6

// When setting the logging level this one is more clear
#define LOGS_NOTHING     0


//-------------------------------------------------------------------------
// Set the level of messages you want to see, default value is LOGS_STATUS
void 
SetLoggingLevel(int newLogLevel);

// Open specified log file
bool 
StartLogFile(const char * fileName);

void 
StopTraceLog();

//-------------------------------------------------------------------------

#define SHOW_TYPE     BIT(0)
#define TO_SYSLOG     BIT(1)
#define TO_COUT       BIT(2)
#define TO_CERR       BIT(3)
#define TO_FILE       BIT(4)

//-------------------------------------------------------------------------

class Multibuf: public streambuf
{
public:
    Multibuf(int logLevel,string type, long flags = SHOW_TYPE|TO_COUT|TO_FILE);
    ~Multibuf();
    
protected:
	int overflow(int ch);
    int underflow();
    int xsputn(char const *str, int size);
    int sync();

private:
    Multibuf();                         // No default constructor
    Multibuf(Multibuf const &);         // No copy constructing
	void operator= (Multibuf const &);  // No copy assignment
    
    map<int,string> fBuffers;
    int    fLogLevel;
    string fType;
    long   fFlags;
};

//-------------------------------------------------------------------------

class MultiStream_pbase
{
public:
    MultiStream_pbase(int logLevel,string type, long flags = SHOW_TYPE|TO_COUT|TO_FILE);
    Multibuf fSbuf;
private:
    MultiStream_pbase();                          // No default constructor
    MultiStream_pbase(MultiStream_pbase const &); // No copy constructing
	void operator= (MultiStream_pbase const &);   // No copy assignment
};

//-------------------------------------------------------------------------

class MultiStream: private MultiStream_pbase, public ostream
{
public:
    MultiStream(int logLevel,string type, long flags = SHOW_TYPE|TO_COUT|TO_FILE);
    const Multibuf *rdbuf() const;
private:
    MultiStream();                         // No default constructor
    MultiStream(MultiStream const &);      // No copy constructing
	void operator= (MultiStream const &);  // No copy assignment
};

//-------------------------------------------------------------------------

#endif // __MULTISTREAM_H__

// End of the file MultiStream.h
//=========================================================================
