/***************************************************************************
                          TraceLog.cpp  -  description                              
                             -------------------                                          
    begin                : Tue Jul 6 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <varargs.h>
#include <iostream.h>

#include "debugging.h"
#include "TraceLog.h"

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef WIN32
#define string std::string
#define snprintf  _snprintf
#define vsnprintf _vsnprintf

#endif


#define MAX_LOG_MESSAGE_SIZE 4096

//-------------------------------------------------------------------------

static int theActualLogLevel = LOG_DEBUG;
static FILE* traceLogFile = NULL;

//-------------------------------------------------------------------------

FUNCTION_START(void SetCurrentLogLevel(int newLogLevel))
{
    theActualLogLevel = newLogLevel;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(int lprintf(int logLevel, const char *format, ...))
{
    int printedChars = 0;
 
return 0;	

    if (theActualLogLevel >= logLevel)
    {
#ifdef WIN32
		va_dcl;
#endif
        va_list ap;

#ifdef WIN32
        va_start(ap);
#else
        va_start(ap, format);
#endif

        time_t now = time(NULL);

        char * time_str = strdup(ctime(&now));
        // Kick out the \n at the end.
        time_str[24] = 0x00;

        char * buffer= (char*)malloc(MAX_LOG_MESSAGE_SIZE*sizeof(char));
        
        printedChars += snprintf(buffer,MAX_LOG_MESSAGE_SIZE, 
                                  "[%s](%02ld): ",time_str,logLevel);

        cout << buffer;
        
        if (traceLogFile != NULL)
            fprintf(traceLogFile,"%s",buffer);
        
        printedChars += vsnprintf(buffer,MAX_LOG_MESSAGE_SIZE, format, ap);

        cout << buffer << endl;

        if (traceLogFile != NULL)
            fprintf(traceLogFile,"%s\n",buffer);

        free (buffer);
        free (time_str);

        va_end(ap);
    }
    return printedChars;

}
FUNCTION_END

//-------------------------------------------------------------------------
// This adds an additional trace of all information that is printed by lprintf

FUNCTION_START(bool StartTraceLog(const char * fileName))
{
    if (fileName == NULL)
    {
        lprintf(LOG_ERROR,"You called StartLogTrace with a NULL filename.");
        return false;
    }

    // Just make sure there is no trace log file left open
    StopTraceLog();
    
    traceLogFile = fopen(fileName,"wt");
    
    if (traceLogFile == NULL)
    {
        lprintf(LOG_ERROR,"Unable to open trace log file %s",fileName);
        return false;
    }
    
    return true;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(void StopTraceLog())
{
    if (traceLogFile != NULL)
        fclose (traceLogFile);
    traceLogFile = NULL;
}
FUNCTION_END

//-------------------------------------------------------------------------
