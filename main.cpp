//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : main.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/12/13 20:09:45 $
//  $Revision: 1.12 $
//  $RCSfile: main.cpp,v $
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//-------------------------------------------------------------------------

#include <time.h>
#include <signal.h>
#include <iostream.h>
#include <iomanip.h>
#include "TraceLog.h"
#include "NNTPRetrieveManager.h"
#include "StatisticsKeeper.h"
#include "IniFile.h"
#include "SuckDefines.h"

//-------------------------------------------------------------------------

void
Copyright()
{
    cout << setprecision(2) // for the version displaying
         << "+==================================================+" << endl
         << "| Suck MT "<< SUCKMT_VERSION <<" - A Multi Threaded suck replacement |" << endl
         << "+--------------------------------------------------+" << endl
         << "| (C)1999 by Niels Basjes  -  http://go.to/suckmt  |" << endl
         << "+==================================================+" << endl;
}

//-------------------------------------------------------------------------

void
Usage()
{
    cout << "+==================================================+" << endl
         << "| Usage: suckmt [-i <file>] [-n <group>] [-init]   |" << endl
         << "| -i <filename>   Use specified file as ini file   |" << endl
         << "| -n <groupname>  Add the specied group            |" << endl
         << "| -init           Fill ini file with defaults      |" << endl
         << "| -q              quiet: don't show the progress   |" << endl
         << "+==================================================+" << endl;
}

//-------------------------------------------------------------------------
void
InitializeIniFile(IniFile &settings)
{
    string dummy;

    // Convenience macro that sets a value to the default if it is not defined
    #define SET_UNDEFINED(key,value,default)\
        if (!settings.GetValue(key,value,dummy))\
            settings.SetValue(key,value,default);
    
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string
 
    // Setting copyright information 
    settings.SetValue(SUCK_COPYRIGHT,SUCK_AUTHOR,
            "Suck MT was written by ir. Niels Basjes (C) 1999");
    settings.SetValue(SUCK_COPYRIGHT,SUCK_LICENSE,
            "Suck MT is distributed under the GNU Public License.");
    settings.SetValue(SUCK_COPYRIGHT,SUCK_WEBSITE,
            "http://go.to/suckmt");
    settings.SetValue(SUCK_COPYRIGHT,SUCK_EMAIL,
            "SuckMT@Basjes.nl");
    
    // Setting default values if they don't exist yet
    SET_UNDEFINED(SUCK_COPYRIGHT,SUCK_FIRST_VERSION,  SUCKMT_VERSION);
    SET_UNDEFINED(SUCK_INSTALL, SUCK_INSTALL_DATE,    nowStr);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_NEWS_SERVER,     "news");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_NNTP_PORT    ,   119);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_NNTP_USERNAME,   "");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_NNTP_PASSWORD,   "");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_DEBUG_SOCKET ,   false);

    SET_UNDEFINED(SUCK_CONFIG,  SUCK_DIR,             "/tmp/");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_BATCH_FILE,      "/tmp/suckmtbatch");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_THREADS,         3);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_SEND_MODEREADER, false);
    
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_LINES,  -1);
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_BYTES,  -1);
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_GROUPS, -1);

    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_ENABLE_LOGFILE, false);
    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILENAME,    "/tmp/SuckMTKillLog.txt");
    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILE_HEADERS,"From Subject Newsgroups Lines X-Trace X-Complaints-To Message-ID");

    // Keep statistics
    settings.SetValue(SUCK_COPYRIGHT,SUCK_LATEST_VERSION,SUCKMT_VERSION);
    settings.SetValue(SUCK_INSTALL,SUCK_LASTRUN_DATE,nowStr);
    settings.AddValue(SUCK_INSTALL,SUCK_RUN,1);

    // Check if the groups section exists
    list<string> groups;
    if (!settings.GetVariableNames(SUCK_GROUPS,groups))
        settings.CreateSection(SUCK_GROUPS);

}

//-------------------------------------------------------------------------

static NNTPRetrieveManager *retrieveManagerToSignal = NULL;

static void 
SuckmtSignalHandler(int /*sig_num*/)
{
    cout << "Received STOP signal." << endl << flush;

    if (retrieveManagerToSignal == NULL)
        exit(1); // No clean signalling option .. just die now.

    retrieveManagerToSignal->Abort(false);
}

//-------------------------------------------------------------------------

static void 
SetSuckmtSignalHandler(NNTPRetrieveManager * nntpRetrieveManager)
{
    retrieveManagerToSignal = nntpRetrieveManager;
    signal(SIGINT,  SuckmtSignalHandler);
//    signal(SIGHUP,  SuckmtSignalHandler);
//    signal(SIGABRT, SuckmtSignalHandler);
//    signal(SIGTERM, SuckmtSignalHandler);
}

//-------------------------------------------------------------------------

int main(int argc, char** argv)
{
    string iniFileName = SUCK_CONFIG_FILE;
    bool   writeDefaultInitFile = false;
    bool   downloadMessages = true;
    bool   showProgress = true;

    IniFile settings;
    
    vector<string> groupsToAdd;
    
//    SetCurrentLogLevel(LOG_EVERYTHING);
//    StartTraceLog("sucknntp.log");

    Copyright();
    
    for (int i = 1 ; i < argc ; i++)
    {
        // INIFILE SETTING ??
        if (!strcmp(argv[i],"-i"))
        {
            if ( argc >= i+2 )
            {
                iniFileName = argv[i+1];
                i++;
                continue;
            }
            else
            {
                cout << "Invalid parameter: Expected a filename after -i" << endl;
                Usage();
                return -1;
            }
        }
        else
        
        // ADD a GROUP ??
        if (!strcmp(argv[i],"-n"))
        {
            if ( argc >= i+2 )
            {
                downloadMessages = false;
                groupsToAdd.push_back(argv[i+1]);
                i++;
                continue;
            }
            else
            {
                cout << "Invalid parameter: Expected a filename after -n" << endl;
                Usage();
                return -1;
            }
        }

        // Create default INIFile ??
        if (!strcmp(argv[i],"-init"))
        {
            writeDefaultInitFile = true;
            continue;
        }

        // Be quiet ??
        if (!strcmp(argv[i],"-q"))
        {
            showProgress = false;
            continue;
        }

        cout << "Unknown commandline parameter " << argv[i] << endl;
        Usage();
        return -1;
    }
    
    cout << "Reading config file \"" << iniFileName.c_str() << "\"" << endl 
         << flush;
         
    if(!settings.ReadFile(iniFileName))
    {
        if (writeDefaultInitFile)
        {
            cout << "Writing a default file settings file \"" << iniFileName << "\"." << endl;
            cout << "Remember to check the settings file." << endl;
            InitializeIniFile(settings);
            settings.WriteFile(iniFileName);
            return -1;
        }
        cout << "Unable to read settings file \"" << iniFileName << "\"." << endl;
        Usage();
        return -1;
    }
    else
    if (writeDefaultInitFile)
    {
        cout << "The specified settings file \"" << iniFileName << "\" "
             << "already exists and will NOT be overwritten." << endl;
        return -1;
    }

    InitializeIniFile(settings);

    vector<string>::iterator newGroupIter;
    
    for ( newGroupIter  = groupsToAdd.begin();
          newGroupIter != groupsToAdd.end();
          newGroupIter++ )
    {
        string dummyValue;
    
        if (settings.GetValue(SUCK_GROUPS,(*newGroupIter),dummyValue))
        {
            cout << "Group \"" << *newGroupIter << "\" already exists." << endl;
        }
        else
        {
            cout << "Adding new group \"" << *newGroupIter << "\"." << endl;
            settings.SetValue(SUCK_GROUPS,(*newGroupIter),-1);
        }
    }

    if (downloadMessages)
    {
        NNTPRetrieveManager nntpRetrieveManager(settings);
    
        SetSuckmtSignalHandler(&nntpRetrieveManager);

        if (showProgress)
            KeepStatistics(10000); // 10000 == Show every 1 second

        nntpRetrieveManager.WaitForCompletion();

        if (showProgress)
        {
            cout << endl;
            //    statisticsKeeper->Print(cout);
            EndStatistics();
        }
        
        // Write the ini file back if we were not aborted
        if (nntpRetrieveManager.KeepRunning())
        {
            cout << "All downloads were completed successfully." << endl << flush;
            settings.WriteFile(iniFileName); 
        }
        else
        {
            cout << "Downloads were aborted by user." << endl << flush;
        }
    }
    else
    {
        settings.WriteFile(iniFileName);
    }
    
    return EXIT_SUCCESS;
}

//-------------------------------------------------------------------------

// End of the file main.cpp
//=========================================================================
