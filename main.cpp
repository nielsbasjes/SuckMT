//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : main.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/04/03 18:41:15 $
//  $Revision: 1.23 $
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
#ifndef WIN32
#include <syslog.h>
#endif
#include <iostream.h>
#include <iomanip.h>
#include "TraceLog.h"
#include "NNTPRetrieveManager.h"
#include "StatisticsKeeper.h"
#include "IniFile.h"
#include "SuckDefines.h"
#include "SetProcTitle.h"

//-------------------------------------------------------------------------

void
Copyright()
{
    Lsyslog << "Starting SuckMT v" << SUCKMT_VERSION << endl << flush;

    Lcout << "+==================================================+" << endl
          << "| Suck MT " << SUCKMT_VERSION 
                         <<" - A Multi Threaded suck replacement |" << endl
          << "+--------------------------------------------------+" << endl
          << "| (C)2000 by Niels Basjes  -  http://go.to/suckmt  |" << endl
          << "+==================================================+" << endl
          << flush;
}

//-------------------------------------------------------------------------

void
Usage()
{
    Lcout << "+==================================================+" << endl
          << "| Usage: suckmt [-i <file>] [-n <group>] [-init]   |" << endl
          << "| -i <filename>   Use specified file as ini file   |" << endl
          << "| -n <groupname>  Add the specied group            |" << endl
          << "| -init           Fill ini file with defaults      |" << endl
          << "| -q              quiet: don't show the progress   |" << endl
          << "+==================================================+" << endl 
          << flush;
}

//-------------------------------------------------------------------------
void
InitializeIniFile(IniFile &settings)
{
    string dummy;

    // Convenience macro that sets a value to the default if it is not defined
    #define SET_UNDEFINED(key,newvalue,default)\
        if (!settings.GetValue(key,newvalue,dummy))\
        {\
            settings.SetValue(key,newvalue,default);\
            Lcout << "Added new value to INI file: [" << key << "] " \
                  << newvalue << " = " << default << endl << flush;\
        }
    
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string
 
    // Setting copyright information 
    settings.SetValue(SUCK_COPYRIGHT,SUCK_AUTHOR,
            "Suck MT was written by ir. Niels Basjes (C) 1999-2000");
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
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_BATCH_FILE,      "/tmp/SuckMT-batch");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_RESTART_FILE,    "/tmp/SuckMT-restart");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_THREADS,         3);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_CONNECT_RETRIES, 0);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_SEND_MODEREADER, false);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_LOGLEVEL,        LOGS_STATUS);
    
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MIN_LINES, "-1 ; 100");
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_LINES, "-1 ; 100");
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_BYTES, "-1 ; 100");
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_GROUPS,"-1 ; 100");

    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_ENABLE_LOGFILE, false);
    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILENAME,    
        "/tmp/SuckMT-KillLog.txt");
    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILE_HEADERS,
        "From Subject Newsgroups Lines X-Trace X-Complaints-To Message-ID");

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
    Levent << "Received STOP signal." << endl << flush;

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

int main(int argc, char** argv, char **envp)
{
    // The system to set the string you see when you do "ps"
    // must be initialised first
#ifndef WIN32
    openlog("SuckMT: ",LOG_PID,LOG_USER);
#endif
    InitSetProcTitle(argc,argv,envp,"SuckMT: ");
    Copyright();

    string iniFileName = SUCK_CONFIG_FILE;
    bool   writeDefaultInitFile = false;
    bool   downloadMessages     = true;
    long   logLevel             = LOGS_STATUS; // Default logging level
    bool   forcedQuiet          = false;

    IniFile settings;
    
    vector<string> groupsToAdd;
    
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
                Lmistake << "Invalid parameter: Expected a filename after -i" 
                         << endl << flush;
                Usage();
                Linfo << "Exit invalid commandline parameter." << endl 
                      << flush;
                return EXIT_FAILURE;
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
                Lmistake << "Invalid parameter: Expected a filename after -n" 
                         << endl << flush;
                Usage();
                Linfo << "Exit invalid commandline parameter." 
                      << endl << flush;
                return EXIT_FAILURE;
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
            SetLoggingLevel(LOGS_NOTHING);
            forcedQuiet = true;
            continue;
        }

        Lmistake << "Unknown commandline parameter :\"" << argv[i] << "\"." 
                 << endl << flush;
        Usage();
        Linfo << "Exit invalid commandline parameter." 
              << endl << flush;
        return EXIT_FAILURE;
    }


    Linfo << "Reading config file \"" << iniFileName.c_str() << "\"" 
          << endl << flush;
         
    if(!settings.ReadFile(iniFileName))
    {
        if (writeDefaultInitFile)
        {
            Linfo << "Writing a default file settings file \"" 
                  << iniFileName << "\"." << endl
                  << "Remember to check the settings file." << endl << flush;
            InitializeIniFile(settings);
            settings.WriteFile(iniFileName);
            Linfo << "Exiting successfully." << endl << flush;
            return EXIT_SUCCESS;
        }
        Lerror << "Unable to read settings file \"" << iniFileName << "\"." 
               << endl << flush;
        Usage();
        Linfo << "Exit no settings file." << endl << flush;
        return EXIT_FAILURE;
    }

    InitializeIniFile(settings);

    if (writeDefaultInitFile)
    {
        settings.WriteFile(iniFileName);
        Linfo << "The specified settings file \"" << iniFileName << "\" " << endl
              << "has been updated to ensure it contains all new " << endl
              << "options exist and have a default value." << endl << flush;
        return EXIT_SUCCESS;
    }

    if (!forcedQuiet)
    {
        if (settings.GetValue(SUCK_CONFIG,SUCK_LOGLEVEL,logLevel))
        {
            SetLoggingLevel(logLevel);
        }
    }

    vector<string>::iterator newGroupIter;
    
    for ( newGroupIter  = groupsToAdd.begin();
          newGroupIter != groupsToAdd.end();
          newGroupIter++ )
    {
        string dummyValue;
    
        if (settings.GetValue(SUCK_GROUPS,(*newGroupIter),dummyValue))
        {
            Lmistake << "Group \"" << *newGroupIter << "\" already exists." 
                     << endl << flush;
        }
        else
        {
            Linfo << "Adding new group \"" << *newGroupIter << "\"." << endl 
                  << flush;
            settings.SetValue(SUCK_GROUPS,(*newGroupIter),-1);
        }
    }

    // If we don't download we can write the ini file now and stop.
    if (!downloadMessages)
    {
        Linfo << "Writing updated config file \"" << iniFileName << "\"." 
              << endl << flush;
        settings.WriteFile(iniFileName);
        Linfo << "Exiting successfully." << endl << flush;
        return EXIT_SUCCESS;
    }

    // ======================
    // Start the downloads...
    // ======================
    KeepStatistics(0); // 0 == Create objects but don't show the values
    
    Linfo << "Beginning message download." << endl << flush;
    NNTPRetrieveManager * nntpRetrieveManager = 
            new NNTPRetrieveManager(settings);

    SetSuckmtSignalHandler(nntpRetrieveManager);
    
    // Wait a short period before starting the logging.
    // This way there will be a few less messages mixedup during startup.
    omni_thread::sleep(0, 500000); // 500000 nanoseconds = 0.5 seconds
    STAT_Start(500000); // 500000 == Show every 0.5 second

    nntpRetrieveManager->WaitForCompletion();

    bool wasAborted = !nntpRetrieveManager->KeepRunning();

    // Tell the user what the ending status was
    if (wasAborted)
    {
        Linfo << "Download aborted by user." << endl << flush;
    }
    else
    {
        Linfo << "Download completed succesfully." << endl << flush;
    }

    // Stop the statistics keeper
    EndStatistics();

    // Delete the retrieve manager
    delete (nntpRetrieveManager);

    if (wasAborted)
    {
        return EXIT_FAILURE;
    }

    // Write the ini file back if we were not aborted
    settings.WriteFile(iniFileName); 
    Linfo << "All downloads were completed successfully." << endl 
          << flush;
    
    Linfo << "Exiting successfully." << endl << flush;
    return EXIT_SUCCESS;
}

//-------------------------------------------------------------------------

// End of the file main.cpp
//=========================================================================
