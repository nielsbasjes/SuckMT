//=========================================================================
//                 Copyright (C)1999-2012 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
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

#include "TraceLog.h"

//=========================================================================
// Create a few default system wide streams
MultiStream Lalways  (LOGS_ALWAYS,  "INFO",              TO_COUT|TO_SYSLOG);
MultiStream Linfo    (LOGS_INFO,    "INFO",              TO_COUT|TO_SYSLOG);
MultiStream Lcout    (LOGS_INFO,    "INFO",              TO_COUT);
MultiStream Lsyslog  (LOGS_INFO,    "INFO",                      TO_SYSLOG);
MultiStream Levent   (LOGS_INFO,    "EVENT",             TO_COUT|TO_SYSLOG);
MultiStream Lmistake (LOGS_ALWAYS,  "ERROR",   SHOW_TYPE|TO_CERR          );
MultiStream Lerror   (LOGS_ALWAYS,  "ERROR",   SHOW_TYPE|TO_CERR|TO_SYSLOG);
MultiStream Lfatal   (LOGS_ALWAYS,  "FATAL",   SHOW_TYPE|TO_CERR|TO_SYSLOG);
MultiStream Lstatus  (LOGS_STATUS,  "STATUS",            TO_COUT);
MultiStream Lverbose (LOGS_VERBOSE, "VERBOSE", SHOW_TYPE|TO_COUT);
MultiStream Lcommands(LOGS_COMMANDS,"COMMANDS",SHOW_TYPE|TO_COUT|TO_FILE);
MultiStream Lcalls   (LOGS_CALLS,   "CALLS",   SHOW_TYPE|TO_COUT|TO_FILE);
MultiStream Lsteps   (LOGS_NOISE,   "STEPS",   SHOW_TYPE|TO_COUT|TO_FILE);

// End of the file TraceLog.cpp
//=========================================================================
