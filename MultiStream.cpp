//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : MultiStream.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 22:19:31 $
//  $Revision: 1.2 $
//  $RCSfile: MultiStream.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#ifndef WIN32
#include <syslog.h>
#include <values.h>
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdarg.h>
#include <iostream.h>
#include <fstream.h>
#include "MultiStream.h"
#include "TraceLog.h"
#include "omnithread.h"

//#define _DEBUG_MULTISTREAM_ 1

#ifdef _DEBUG_MULTISTREAM_
#define D(a) a
#else
#define D(a) 
#endif


//-------------------------------------------------------------------------

int GetThisThreadId()
{
    omni_thread *self = omni_thread::self();
    if (self == NULL)
    {
        return 0;
    }
    else
    {
        return self->id();
    }
} 

//-------------------------------------------------------------------------

static long CurrentLoggingLevel = LOGS_STATUS; // Default value

void SetLoggingLevel(int newLogLevel)
{
    CurrentLoggingLevel = newLogLevel;
}

//=========================================================================
// Log FILE related functions and variables
//=========================================================================

static ofstream traceLogFile;

//-------------------------------------------------------------------------
// Begin logging to the specified log file

bool 
StartLogFile(const char * fileName)
{
    if (fileName == NULL)
    {
        Lerror << "You called StartLogFile with a NULL filename." << endl << flush;
        return false;
    }

    // Just make sure there is no trace log file left open
    StopTraceLog();

    traceLogFile.open(fileName,ios::app);

    if (!traceLogFile.good())
    {
        Lerror << "Unable to open log file " << fileName << endl << flush;
        return false;
    }
    
    return true;
}

//-------------------------------------------------------------------------

void 
StopTraceLog()
{
    if (traceLogFile.good())
        traceLogFile.close();
}

//-------------------------------------------------------------------------

static char loggerBuffer [MAX_LOGGER_SIZE + 100];

omni_mutex loggerMutex;

static void Logger(int logLevel, long flags, string type, string message)
{
    if (logLevel <= CurrentLoggingLevel)
    {
        omni_mutex_lock lock(loggerMutex);
    
        if (message.length() > 1000)
        {
            sprintf(loggerBuffer,
                "Attempt to log a message that is MUCH too large (%d characters).",
                message.length());
        }
        else
        {
            if ((flags & SHOW_TYPE)  != 0)
                sprintf(loggerBuffer,"[%-8s]:%s",type.c_str(),message.c_str());
            else
                sprintf(loggerBuffer,"%s",message.c_str());
 
#ifndef WIN32
            if ((flags & TO_SYSLOG)  != 0)
                syslog(LOG_NOTICE,"%s",loggerBuffer);
#endif                
            if ((flags & TO_COUT)    != 0)
                cout << loggerBuffer << flush;
                
            if ((flags & TO_CERR)   != 0)
                cerr << loggerBuffer << flush;
                
            if ((flags & TO_FILE)   != 0)
                if (traceLogFile.good())
                    traceLogFile << loggerBuffer << flush;
        }
    }
}

//-------------------------------------------------------------------------

Multibuf::Multibuf(int logLevel,string type, long flags)
{
    fLogLevel = logLevel;
    fType     = type;
    fFlags    = flags;
}

//-------------------------------------------------------------------------

Multibuf::~Multibuf()
{
    // Nothing to do
}
    
//-------------------------------------------------------------------------

int 
Multibuf::underflow()
{
    D(cout << "UNDERFLOW" << endl << flush;)
    return 500;
}

//-------------------------------------------------------------------------
int 
Multibuf::overflow(int ch)
{
    D(cout << "OVERFLOW(\'" << (char)ch << "\')" << endl << flush;)
    fBuffers[GetThisThreadId()] += (char)ch;
    return !EOF;
}

//-------------------------------------------------------------------------

int 
Multibuf::xsputn(char const *str, int size)
{
    D(cout << "XSPUTN(\"" << str << "\"," << size << ")" << endl << flush;)
    string realStr(str,size);
    fBuffers[GetThisThreadId()] += realStr;
    return size;
}

//-------------------------------------------------------------------------
int 
Multibuf::sync()
{
    int thisThreadId = GetThisThreadId();
    D(cout << "SYNC: buffer = \""<< fBuffers[thisThreadId].c_str() << "\"." << endl << flush;)
    if (fBuffers[thisThreadId] == "")
        return !EOF;
    Logger(fLogLevel, fFlags, fType, fBuffers[thisThreadId]);
    fBuffers[thisThreadId] = "";
    return !EOF;
}

//-------------------------------------------------------------------------

MultiStream_pbase::MultiStream_pbase(int logLevel,string type, long flags)
    :fSbuf(logLevel,type,flags)
{
    // Nothing else to do
};

//-------------------------------------------------------------------------

MultiStream::MultiStream(int logLevel,string type, long flags)
    :MultiStream_pbase(logLevel,type,flags)
    ,ostream(&fSbuf)
{
    // Nothing else to do
}

//-------------------------------------------------------------------------

const Multibuf *
MultiStream::rdbuf() const 
{ 
    return &fSbuf; 
}
    
//-------------------------------------------------------------------------

// End of the file TraceLog.h
//=========================================================================
