//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : TraceLog.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 21:31:16 $
//  $Revision: 1.4 $
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

#include "MultiStream.h"

// These are a few default system wide streams
extern MultiStream Lalways  ;//(LOGS_ALWAYS,  "INFO",              TO_COUT|TO_SYSLOG);
extern MultiStream Linfo    ;//(LOGS_INFO,    "INFO",              TO_COUT|TO_SYSLOG);
extern MultiStream Lcout    ;//(LOGS_INFO,    "INFO",              TO_COUT);
extern MultiStream Lsyslog  ;//(LOGS_INFO,    "INFO",                      TO_SYSLOG);
extern MultiStream Levent   ;//(LOGS_INFO,    "EVENT",   SHOW_TYPE|TO_COUT|TO_SYSLOG);
extern MultiStream Lmistake ;//(LOGS_ALWAYS,  "ERROR",   SHOW_TYPE|TO_CERR          );
extern MultiStream Lerror   ;//(LOGS_ALWAYS,  "ERROR",   SHOW_TYPE|TO_CERR|TO_SYSLOG);
extern MultiStream Lfatal   ;//(LOGS_ALWAYS,  "FATAL",   SHOW_TYPE|TO_CERR|TO_SYSLOG);
extern MultiStream Lstatus  ;//(LOGS_STATUS,  "STATUS",            TO_COUT);
extern MultiStream Lverbose ;//(LOGS_STATUS,  "STATUS",  SHOW_TYPE|TO_COUT);
extern MultiStream Lcommands;//(LOGS_COMMANDS,"COMMANDS",SHOW_TYPE|TO_COUT|TO_FILE);
extern MultiStream Lcalls   ;//(LOGS_CALLS,   "CALLS",   SHOW_TYPE|TO_COUT|TO_FILE);
extern MultiStream Lsteps   ;//(LOGS_NOISE,   "STEPS",   SHOW_TYPE|TO_COUT|TO_FILE);

//-------------------------------------------------------------------------

#endif // __TRACELOG_H__

// End of the file TraceLog.h
//=========================================================================
