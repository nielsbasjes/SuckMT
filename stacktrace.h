//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : stacktrace.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:28:09 $
//  $Revision: 1.2 $
//  $RCSfile: stacktrace.h,v $
//  $Author: niels $
//=========================================================================

#ifndef STACKTRACE_H
#define STACKTRACE_H

//-------------------------------------------------------------------------

#include <string>
#include <iostream.h>

//-------------------------------------------------------------------------

#ifdef __WIN32__
#define string std::string
#endif

//-------------------------------------------------------------------------

class StackTrace {
public: 
    StackTrace(const char * functionName,const char * filename,const int linenumber);
    ~StackTrace();
    void PrintStackTrace();
    unsigned int GetStackLevel();
private:
    string function_name;
    string source_file_of_function;
    int    source_file_linenr;
    unsigned int stackLevel;
    StackTrace * caller;
};

//-------------------------------------------------------------------------

#ifdef DEBUG_TRACECALLS

#define FUNCTION(functionDeclaration) \
        functionDeclaration\
        { \
        StackTrace TRACE__stackTraceObject(#functionDeclaration,__FILE__,__LINE__); \
        string     TRACE__theNameOfThisFunction = #functionDeclaration; \
        try \

#define FUNCTION_START(functionDeclaration) FUNCTION(functionDeclaration)

#define FUNCTION_END \
        catch(...) \
        { \
            cout << "## [TRACE EXCEPT](" \
                 << TRACE__stackTraceObject.GetStackLevel() \
                 << "): Ending function \"" << TRACE__theNameOfThisFunction \
                 << "\" because an exception ocurred." << endl << flush;\
            throw; \
        } }

#define PrintStack() TRACE__stackTraceObject.PrintStackTrace()

#define NOT_YET_IMPLEMENTED(os) \
            os << "[WARNING](" \
                 << TRACE__stackTraceObject.GetStackLevel() \
                 << "): The function \"" << TRACE__theNameOfThisFunction \
                 << "\" has NOT YET BEEN IMPLEMENTED !!!!" << endl << flush;\

#else

#define PrintStack()
#define FUNCTION(functionDeclaration) functionDeclaration
#define FUNCTION_START(functionDeclaration) functionDeclaration
#define FUNCTION_END
#define NOT_YET_IMPLEMENTED(os) cout << "[WARNING]: Called a function in" << __FILE__ << "@" << __LINE__ << " that has not yet been implemented." << endl << flush;

#endif

//-------------------------------------------------------------------------
#endif

// End of the file stacktrace.h
//=========================================================================
