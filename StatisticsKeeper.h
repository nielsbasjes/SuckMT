//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : StatisticsKeeper.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:52:27 $
//  $Revision: 1.4 $
//  $RCSfile: StatisticsKeeper.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __STATISTICSKEEPER_H__
#define __STATISTICSKEEPER_H__

//-------------------------------------------------------------------------

#include <map>
#include "omnithread.h"
#include "Printable.h"
#include "Abortable.h"

//-------------------------------------------------------------------------

#ifdef __WIN32__
#define map std::map
#endif

//-------------------------------------------------------------------------

class StatisticsKeeper : public omni_thread, 
                         public Printable, 
                         public Abortable
{
public: 
    StatisticsKeeper(long milliseconds);
    ~StatisticsKeeper();
    
    void SetValue(string name, string value);
    void SetValue(string name, long value);
    void AddValue(string name, long value);
    
    string GetStringValue(string name);
    long GetNumericValue(string name);

    void Print(ostream &os);

private:
    omni_mutex          valuesMutex;
    map <string,long>   numericValues;
    map <string,string> stringValues;
    
    void* run_undetached(void* arg);
    
    long fMilliseconds;
    
    bool valuesModified;
};

//-------------------------------------------------------------------------

// The specified time indicated the statistics should be printer automagically
void KeepStatistics(long milliseconds);
void EndStatistics();

void STAT_SetValue(string name, string value);
void STAT_SetValue(string name, long value);
void STAT_AddValue(string name, long value);
    
string STAT_GetStringValue(string name);
long   STAT_GetNumericValue(string name);

extern StatisticsKeeper * statisticsKeeper;

//-------------------------------------------------------------------------

#endif // __STATISTICSKEEPER_H__

// End of the file StatisticsKeeper.h
//=========================================================================
