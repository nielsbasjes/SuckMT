/***************************************************************************
                          StatisticsKeeper.cpp  -  description                              
                             -------------------                                         
    begin                : Sat Jul 31 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include "debugging.h"
#include "StatisticsKeeper.h"
 
//--------------------------------------------------------------------

StatisticsKeeper * statisticsKeeper = NULL;

//--------------------------------------------------------------------

// The specified time indicated the statistics should be printer automagically
void 
FUNCTION_START(KeepStatistics(long milliseconds))
{
    if (statisticsKeeper != NULL)
        EndStatistics();
    
    statisticsKeeper = new StatisticsKeeper(milliseconds);
}
FUNCTION_END

//--------------------------------------------------------------------

void 
FUNCTION_START(EndStatistics())
{
    if (statisticsKeeper == NULL)
        return;
        
    statisticsKeeper->Abort();
    statisticsKeeper->join(NULL);
    statisticsKeeper = NULL;
}

//--------------------------------------------------------------------

FUNCTION_START(StatisticsKeeper::StatisticsKeeper(long milliseconds): Printable("StatisticsKeeper"))
{
    valuesModified = true;
    fMilliseconds = milliseconds;
    if (fMilliseconds > 0)
        start_undetached();
}
FUNCTION_END

//--------------------------------------------------------------------

FUNCTION_START(StatisticsKeeper::~StatisticsKeeper())
{
    // Nothing to do
}
FUNCTION_END

//-------------------------------------------------------------------------

void* 
FUNCTION_START(StatisticsKeeper::run_undetached(void* /*arg*/))
{
    unsigned long sleepTime = fMilliseconds * 1000;
    while (KeepRunning())
    {
        sleep(0,sleepTime);    
        if(valuesModified)
        {
            unsigned long killed  = GetNumericValue("Articles Killed");
            unsigned long total   = GetNumericValue("Articles Present");
            unsigned long stored  = GetNumericValue("Articles Written");
            unsigned long skipped = GetNumericValue("Articles Skipped");
            unsigned long qlen    = GetNumericValue("Command Queue Length");
            unsigned long done    = killed+stored+skipped;
            float precentage_done = (total==0?0.0:(((float)(done))/((float)total))*100.0);

            // I couldn't get the stream formatting to do what I wanted
            // So let's do this the traditional way.
            char percentage[100];
            sprintf(percentage,"%3.0f",precentage_done);

            cout 
                << "Q(" << setw(5) << qlen <<")," // Queue length
                << "Total("  << setw(5) << total  <<")," // Total
                << "Done (" << percentage <<"%)={" // Killed
                << "Stor("  << setw(5) << stored  <<")," // Stored
                << "Skip(" << setw(5) << skipped <<")," // Skipped
                << "Kill("  << setw(5) << killed  <<")}   \r" // Killed
                << flush;
            
            valuesModified = false;
        }
    }

    cout << endl << flush;

    return NULL;
}
FUNCTION_END


//--------------------------------------------------------------------

void
FUNCTION_START(StatisticsKeeper::SetValue(string name, string value))
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;
    stringValues[name] = value;
}
FUNCTION_END

//--------------------------------------------------------------------

void
FUNCTION_START(StatisticsKeeper::SetValue(string name, long value))
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;
    numericValues[name] = value;
}
FUNCTION_END

//--------------------------------------------------------------------

void
FUNCTION_START(StatisticsKeeper::AddValue(string name, long value))
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;

    numericValues[name] = numericValues[name] + value;
}
FUNCTION_END

//--------------------------------------------------------------------

string 
FUNCTION_START(StatisticsKeeper::GetStringValue(string name))
{
    omni_mutex_lock lock(valuesMutex);

    return stringValues[name];
}
FUNCTION_END

//--------------------------------------------------------------------

long 
FUNCTION_START(StatisticsKeeper::GetNumericValue(string name))
{
    omni_mutex_lock lock(valuesMutex);

    return numericValues[name];
}
FUNCTION_END

//--------------------------------------------------------------------

void 
FUNCTION_START(StatisticsKeeper::Print(ostream &os))
{
    omni_mutex_lock lock(valuesMutex);

    map <string,string>::const_iterator stringValuesIter;

    os << "Statistic values= " << endl << "{" << endl;

    for(stringValuesIter  = stringValues.begin();
        stringValuesIter != stringValues.end();
        stringValuesIter ++)
    {
        os << "\t" << stringValuesIter->first 
           << " \t= \"" << stringValuesIter->second << "\"" << endl;
    }

    map <string,long>::const_iterator   numericValuesIter;

    for(numericValuesIter  = numericValues.begin();
        numericValuesIter != numericValues.end();
        numericValuesIter ++)
    {
        os << "\t" << numericValuesIter->first 
           << " \t= \"" << numericValuesIter->second << "\"" << endl;
    }
    
    os << "}" << endl;
}
FUNCTION_END

//--------------------------------------------------------------------

void 
FUNCTION_START(STAT_SetValue(string name, string value))
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->SetValue(name,value);
}
FUNCTION_END

//--------------------------------------------------------------------

void 
FUNCTION_START(STAT_SetValue(string name, long value))
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->SetValue(name,value);
}
FUNCTION_END

//--------------------------------------------------------------------

void 
FUNCTION_START(STAT_AddValue(string name, long value))
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->AddValue(name,value);
}
FUNCTION_END

//--------------------------------------------------------------------

string
FUNCTION_START(STAT_GetStringValue(string name))
{
    if (statisticsKeeper == NULL)
        return "Statistics are disabled";
    return statisticsKeeper->GetStringValue(name);
}
FUNCTION_END

//--------------------------------------------------------------------

long
FUNCTION_START(STAT_GetNumericValue(string name))
{
    if (statisticsKeeper == NULL)
        return -1;
    return statisticsKeeper->GetNumericValue(name);
}
FUNCTION_END

//--------------------------------------------------------------------
