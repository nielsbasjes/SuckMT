//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: MultiStream.cpp 17 2008-08-17 15:29:35Z niels $
//  $Date: 2008-08-17 17:29:35 +0200 (Sun, 17 Aug 2008) $
//  $Revision: 17 $
//  $LastChangedBy: niels $
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

#ifdef _MSC_VER
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
#include <iostream>
#include <fstream>
#include "MultiStream.h"
#include "TraceLog.h"
#include <omnithread.h>

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

static char screenBuffer [MAX_LOGGER_SIZE + 100];
static char loggerBuffer [MAX_LOGGER_SIZE + 100];

static omni_mutex loggerMutex;

// ThreadID of the thread that 'owns' the current screen line
// -1 means NO owner
static int currentScreenLineOwner = -1; 

static void Logger(int logLevel, long flags, string type, string message)
{
    if (logLevel <= CurrentLoggingLevel)
    {
//        omni_mutex_lock lock(loggerMutex);
        int thisThread = GetThisThreadId();
        bool prependNewLine = false;
        
        // Is someone is the current owner ??
        if (currentScreenLineOwner != -1)
        {   // YES !!

            // Am I that someone (i.e. the current owner) ??
            if (thisThread != currentScreenLineOwner)
            {   // NO !!
                // In this case we prepend a newline to avoid mixing 
                // lines with the other threads
                prependNewLine = true;
            }
        }

        // If the last character wasn't a newline this means this thread 
        // must become the owner of this line
        if (*(message.end()-1) != '\n')
        {
            currentScreenLineOwner = thisThread;
        }
        else
        {
            currentScreenLineOwner = -1;
        }

        if (message.length() > 1000)
        {
            sprintf(screenBuffer,
                "Attempt to log a message that is MUCH too large (%d characters).\n",
                message.length());
            sprintf(loggerBuffer,
                "Attempt to log a message that is MUCH too large (%d characters).\n",
                message.length());
        }
        else
        {
            if ((flags & SHOW_TYPE)  != 0)
            {
                sprintf(screenBuffer,"%s[%-8s]:%s",prependNewLine?"\n":"",type.c_str(),message.c_str());
                sprintf(loggerBuffer,"[%-8s]:%s",type.c_str(),message.c_str());
            }
            else
            {
                sprintf(screenBuffer,"%s%s",prependNewLine?"\n":"",message.c_str());
                sprintf(loggerBuffer,"%s",message.c_str());
            }

#ifndef WIN32
            if ((flags & TO_SYSLOG)  != 0)
                syslog(LOG_NOTICE,"%s",loggerBuffer);
#endif                
            if ((flags & TO_COUT)    != 0)
                cout << screenBuffer << flush;
                
            if ((flags & TO_CERR)   != 0)
                cerr << screenBuffer << flush;
                
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
    omni_mutex_lock lock(loggerMutex);
    D(cout << "UNDERFLOW" << endl << flush;)
    return 500;
}

//-------------------------------------------------------------------------
int 
Multibuf::overflow(int ch)
{
    omni_mutex_lock lock(loggerMutex);
    D(cout << "OVERFLOW(\'" << (char)ch << "\')" << endl << flush;)
    fBuffers[GetThisThreadId()] += (char)ch;
    return !EOF;
}

//-------------------------------------------------------------------------

int 
Multibuf::xsputn(char const *str, int size)
{
    omni_mutex_lock lock(loggerMutex);
    D(cout << "XSPUTN(\"" << str << "\"," << size << ")" << endl << flush;)
    string realStr(str,size);
    fBuffers[GetThisThreadId()] += realStr;
    return size;
}

//-------------------------------------------------------------------------
int 
Multibuf::sync()
{
    omni_mutex_lock lock(loggerMutex);
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
}

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
