//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : StatisticsKeeper.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2002/12/27 13:24:55 $
//  $Revision: 1.7 $
//  $RCSfile: StatisticsKeeper.h,v $
//  $Author: niels $
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

#ifndef __STATISTICSKEEPER_H__
#define __STATISTICSKEEPER_H__

//-------------------------------------------------------------------------

#include <map>
#include <omnithread.h>
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
    StatisticsKeeper(long milliseconds = 0);
    ~StatisticsKeeper();
    
    void Start(long milliseconds);
    
    void SetValue(string name, string value);
    void SetValue(string name, long value);
    void AddValue(string name, long value);
    
    string GetStringValue(string name);
    long GetNumericValue(string name);

    virtual void Print(ostream &os) const;

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

void STAT_Start(long milliseconds);
void STAT_SetValue(string name, string value);
void STAT_SetValue(string name, long value);
void STAT_AddValue(string name, long value);
    
string STAT_GetStringValue(string name);
long   STAT_GetNumericValue(string name);

//-------------------------------------------------------------------------

#endif // __STATISTICSKEEPER_H__

// End of the file StatisticsKeeper.h
//=========================================================================
