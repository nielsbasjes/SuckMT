/***************************************************************************
                          TraceLog.h  -  description                              
                             -------------------                                         
    begin                : Tue Jul 6 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef __TraceLog_h__
#define __TraceLog_h__

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

#endif // __TraceLog_h__


