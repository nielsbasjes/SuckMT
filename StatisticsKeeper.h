
/***************************************************************************
                          StatisticsKeeper.h  -  description                              
                             -------------------                                         
    begin                : Sat Jul 31 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/


#ifndef STATISTICSKEEPER_H
#define STATISTICSKEEPER_H

#include <map>
#include "omnithread.h"
#include "Printable.h"
#include "Abortable.h"

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define map std::map
#endif

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

// The specified time indicated the statistics should be printer automagically
void KeepStatistics(long milliseconds);
void EndStatistics();

void STAT_SetValue(string name, string value);
void STAT_SetValue(string name, long value);
void STAT_AddValue(string name, long value);
    
string STAT_GetStringValue(string name);
long   STAT_GetNumericValue(string name);

extern StatisticsKeeper * statisticsKeeper;

#endif
