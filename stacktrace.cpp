/***************************************************************************
                          stacktrace.cpp  -  description                              
                             -------------------                                         
    begin                : Wed Jul 7 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include "stacktrace.h"

static unsigned int currentStackLevel = 0;

StackTrace * topOfStack = NULL;

ostream * traceStream = &cout;

static unsigned int lognr=0;

//-------------------------------------------------------------------------

StackTrace::StackTrace(const char * functionName,const char * filename,const int linenumber)
{
    currentStackLevel++;
    caller                  = topOfStack;
    topOfStack              = this;
    stackLevel              = currentStackLevel;
    function_name           = functionName;
    source_file_of_function = filename;
    source_file_linenr      = linenumber;

    traceStream->width(4);
    traceStream->fill('_');
    *traceStream << "## (" << ++lognr 
                 << ") [TRACE ENTER ](" << stackLevel << "):";
    traceStream->width(0);

    int i = currentStackLevel-1;
    while (i-- > 0) 
        *traceStream << "   ";

    *traceStream << ">>>";
        
    *traceStream 
        << ":" << function_name.c_str() 
        << " @ ( " << source_file_of_function.c_str() << ":" << source_file_linenr <<" )" 
        << endl << flush;
}

//-------------------------------------------------------------------------

StackTrace::~StackTrace()
{
    traceStream->width(4);
    traceStream->fill('_');
    *traceStream << "## (" << ++lognr 
                 << ") [TRACE LEAVE ](" << stackLevel << "):";
    traceStream->width(0);

    int i = currentStackLevel-1;
    while (i-- > 0) 
        *traceStream << "   ";

    *traceStream << "<<<";
    
    *traceStream 
        << ":" << function_name.c_str()
        << " @ ( " << source_file_of_function.c_str() << ":" << source_file_linenr <<" )" 
        << endl << flush;

    currentStackLevel--;
    topOfStack = caller;
}

//-------------------------------------------------------------------------

unsigned int StackTrace::GetStackLevel()
{
    return stackLevel;
}

//-------------------------------------------------------------------------

void StackTrace::PrintStackTrace()
{
    if (this == topOfStack)
    {
        traceStream->width(4);
        traceStream->fill('_');
        *traceStream 
            << "#### (" << ++lognr 
            << ") [TRACE STACK ](" << stackLevel << "):" << endl
            << "#### ======================" << endl
            << "#### CALL STACK:" << endl;
        traceStream->width(0);
    }
    
    *traceStream << "#### -- (" << stackLevel << "): " << function_name.c_str() << endl;
    if (caller != NULL)
        caller->PrintStackTrace();
    
    if (this == topOfStack)
    {
        *traceStream 
        << "#### ======================" << endl;
    }
    
    *traceStream << flush;
}

//-------------------------------------------------------------------------
