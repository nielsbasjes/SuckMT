//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : TraceLog.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:51:42 $
//  $Revision: 1.3 $
//  $RCSfile: TraceLog.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __TRACELOG_H__
#define __TRACELOG_H__

//-------------------------------------------------------------------------

// Sets the current log level
void SetCurrentLogLevel(int newLogLevel);

// This function only prints if the current loglevel 
// is higher or equal to the specified loglevel
// This means that increasing the loglevel will let more messages through.
// The information is output through cout
int lprintf(int logLevel, const char *format, ...);

// This adds an additional trace of all information that is printed by lprintf
bool StartTraceLog(const char * fileName);
void StopTraceLog();

//-------------------------------------------------------------------------

// Set of predefined log levels (just for convienience)
// These are just some values I find convenient.
#define LOG_MESSAGE      0
#define LOG_FATAL        0
#define LOG_ERROR       10
#define LOG_ALERT       20
#define LOG_WARNING     30
#define LOG_NOTICE      40
#define LOG_DEBUG       50
#define LOG_CALLTRACE   60
#define LOG_SOCKET      70
#define LOG_EVERYTHING  65000

//-------------------------------------------------------------------------

#endif // __TRACELOG_H__

// End of the file TraceLog.h
//=========================================================================
