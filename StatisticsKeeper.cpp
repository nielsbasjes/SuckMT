//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : StatisticsKeeper.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/04/04 11:22:36 $
//  $Revision: 1.10 $
//  $RCSfile: StatisticsKeeper.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <time.h>
#include <stdio.h>
#include <iostream.h>
#include <iomanip.h>
#include "omnithread.h"
#include "StatisticsKeeper.h"
#include "SetProcTitle.h"
#include "TraceLog.h"

//--------------------------------------------------------------------

StatisticsKeeper * statisticsKeeper = NULL;

//--------------------------------------------------------------------

// The specified time indicates how often the statistics should be 
// printed automagically
void 
KeepStatistics(long milliseconds)
{
    if (statisticsKeeper != NULL)
        EndStatistics();
    
    statisticsKeeper = new StatisticsKeeper(milliseconds);
}

//--------------------------------------------------------------------

void 
EndStatistics()
{
    if (statisticsKeeper == NULL)
        return;
        
    statisticsKeeper->Abort();
    // The call to join does a "delete this" at the end.
    statisticsKeeper->join(NULL);
    statisticsKeeper = NULL;
}

//--------------------------------------------------------------------
StatisticsKeeper::StatisticsKeeper(long milliseconds)
    :Printable("StatisticsKeeper")
{
    valuesModified = true;
    fMilliseconds = milliseconds;
    Start(milliseconds);
}

//--------------------------------------------------------------------

StatisticsKeeper::~StatisticsKeeper()
{
    // Nothing to do
}

//-------------------------------------------------------------------------

void 
StatisticsKeeper::Start(long milliseconds)
{
    fMilliseconds = milliseconds;
    if (fMilliseconds > 0)
        start_undetached();
}

//-------------------------------------------------------------------------

void* 
StatisticsKeeper::run_undetached(void* /*arg*/)
{
    unsigned long sleepTime = fMilliseconds * 1000;
    time_t        startTime = time(NULL);
    time_t        usedTime; 

    unsigned long qlen    ;
    unsigned long killed  ;
    unsigned long total   ;
    unsigned long stored  ;
    unsigned long skipped ;
    unsigned long msgerror;
    unsigned long done    ;
    float precentage_done ;
    char  percentage[100] ;
    unsigned long bytesReceived;
    unsigned long bytesSent;
    unsigned long avgBytesReceivedPerSecond;

    while (KeepRunning())
    {
        omni_thread::sleep(0,sleepTime);
        if(valuesModified)
        {
            qlen    = GetNumericValue("Command Queue Length");
            killed  = GetNumericValue("Articles Killed");
            total   = GetNumericValue("Articles Present");
            stored  = GetNumericValue("Articles Written");
            skipped = GetNumericValue("Articles Skipped");
            msgerror= GetNumericValue("Articles ERROR");
            done    = killed+stored+skipped+msgerror;

            precentage_done = 
                (total==0?0.0:(((float)(done))/((float)total))*100.0);

            // I couldn't get the stream formatting to do what I wanted
            // So let's do this the traditional way.
            sprintf(percentage,"%3.0f",precentage_done);

            Lstatus 
                << "MSGS:"  << setw(4) << total      <<","    // Total
                << "TODO:"  << setw(4) << qlen       <<","    // Queue length
                << "DONE:"             << percentage << "%={" 
                << "STOR:"  << setw(4) << stored     <<","    // Stored
                << "SKIP:"  << setw(4) << skipped    <<","    // Skipped
                << "KILL:"  << setw(4) << killed     <<","    // Killed
                << "ERROR:" << setw(4) << msgerror   <<"} \r" // Error
                << flush;

            bytesReceived = GetNumericValue("Socket Received Bytes");
                    
            usedTime = time(NULL) - startTime;
            if (usedTime==0)
                avgBytesReceivedPerSecond = bytesReceived; // assume 1 second
            else
                avgBytesReceivedPerSecond = (bytesReceived)/(usedTime);

            SetProcTitle("%3d%% (%4d/%4d) @ Average %6d bytes/second.    ",
                    (unsigned long)precentage_done,done,total,
                    avgBytesReceivedPerSecond);
            
            valuesModified = false;
        }
    }

    Linfo << endl << flush;

    // ----------------------------
    // Print the overall statistics
    bytesSent     = GetNumericValue("Socket Send Bytes");
    bytesReceived = GetNumericValue("Socket Received Bytes");
    
    usedTime = time(NULL) - startTime;
    if (usedTime==0)
        avgBytesReceivedPerSecond = bytesReceived; // assume 1 second
    else
        avgBytesReceivedPerSecond = (bytesReceived)/(usedTime);

    Linfo << "Overall : ";
    Linfo << "OUT: " << setw(7) << bytesSent << " bytes, " 
          << "IN: " << setw(9) <<  bytesReceived << " bytes, "
          << "AVG: " << setw(7) << avgBytesReceivedPerSecond << " bytes/second" 
          << endl << flush;

    return NULL;
}


//--------------------------------------------------------------------

void
StatisticsKeeper::SetValue(string name, string value)
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;
    stringValues[name] = value;
}

//--------------------------------------------------------------------

void
StatisticsKeeper::SetValue(string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;
    numericValues[name] = value;
}

//--------------------------------------------------------------------

void
StatisticsKeeper::AddValue(string name, long value)
{
    omni_mutex_lock lock(valuesMutex);
    valuesModified = true;

    numericValues[name] = numericValues[name] + value;
}

//--------------------------------------------------------------------

string 
StatisticsKeeper::GetStringValue(string name)
{
    omni_mutex_lock lock(valuesMutex);

    return stringValues[name];
}

//--------------------------------------------------------------------

long 
StatisticsKeeper::GetNumericValue(string name)
{
    omni_mutex_lock lock(valuesMutex);

    return numericValues[name];
}

//--------------------------------------------------------------------

void 
StatisticsKeeper::Print(ostream &os)
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

//--------------------------------------------------------------------

void 
STAT_Start(long milliseconds)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->Start(milliseconds);
}

//-------------------------------------------------------------------------

void 
STAT_SetValue(string name, string value)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->SetValue(name,value);
}

//--------------------------------------------------------------------

void 
STAT_SetValue(string name, long value)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->SetValue(name,value);
}

//--------------------------------------------------------------------

void 
STAT_AddValue(string name, long value)
{
    if (statisticsKeeper == NULL)
        return;
    statisticsKeeper->AddValue(name,value);
}

//--------------------------------------------------------------------

string
STAT_GetStringValue(string name)
{
    if (statisticsKeeper == NULL)
        return "Statistics are disabled";
    return statisticsKeeper->GetStringValue(name);
}

//--------------------------------------------------------------------

long
STAT_GetNumericValue(string name)
{
    if (statisticsKeeper == NULL)
        return -1;
    return statisticsKeeper->GetNumericValue(name);
}

//--------------------------------------------------------------------

// End of the file StatisticsKeeper.cpp
//=========================================================================
