//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: main.cpp 8 2004-12-20 22:15:34Z niels $
//  $Date: 2004-12-20 23:15:34 +0100 (Mon, 20 Dec 2004) $
//  $Revision: 8 $
//  $LastChangedBy: niels $
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

#ifdef _MSC_VER
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
#include <iostream>
#include <iomanip>
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
          << "| SuckMT " << SUCKMT_VERSION 
                        <<" - A Multi Threaded suck replacement. |" << endl
          << "+--------------------------------------------------+" << endl
          << "| (C)2005 by Niels Basjes  http://suckmt.basjes.nl |" << endl
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
    
    #define SET_UNDEF_STR(key,newvalue,default)\
        if (!settings.GetValue(key,newvalue,dummy))\
        {\
            dummy = default;\
            settings.SetValue(key,newvalue,dummy);\
            Lcout << "Added new value to INI file: [" << key << "] " \
                  << newvalue << " = " << default << endl << flush;\
        }
    
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string
 
    // Setting copyright information 
    settings.SetValue(SUCK_COPYRIGHT,SUCK_AUTHOR,
            string("SuckMT was written by drs. ir. Niels Basjes (C) 1999-2005"));
    settings.SetValue(SUCK_COPYRIGHT,SUCK_LICENSE,
            string("SuckMT is distributed under the GNU Public License."));
    settings.SetValue(SUCK_COPYRIGHT,SUCK_WEBSITE,
            string("http://suckmt.basjes.nl"));
    settings.SetValue(SUCK_COPYRIGHT,SUCK_EMAIL,
            string("SuckMT@Basjes.nl"));
    
    // Setting default values if they don't exist yet
    SET_UNDEF_STR(SUCK_COPYRIGHT,SUCK_FIRST_VERSION,  SUCKMT_VERSION);
    SET_UNDEF_STR(SUCK_INSTALL, SUCK_INSTALL_DATE,    nowStr);
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_NEWS_SERVER,     "news");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_NNTP_PORT    ,   119);
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_NNTP_USERNAME,   "");
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_NNTP_PASSWORD,   "");
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_KEEP_CR,         false);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_DEBUG_SOCKET ,   false);

#ifdef WIN32
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_DIR,             "/tmp/");
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_BATCH_FILE,      "/tmp/SuckMT-batch");
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_RESTART_FILE,    "/tmp/SuckMT-restart");
#else
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_DIR,             "/var/spool/suckmt/in.coming");
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_BATCH_FILE,      "/var/spool/suckmt/batch");
    SET_UNDEF_STR(SUCK_CONFIG,  SUCK_RESTART_FILE,    "/var/spool/suckmt/restart");
#endif    
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_THREADS,         3);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_CONNECT_RETRIES, 0);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_SEND_MODEREADER, true);
    SET_UNDEFINED(SUCK_CONFIG,  SUCK_LOGLEVEL,        LOGS_STATUS);
    
    SET_UNDEF_STR(SUCK_GLOBAL_KILL_RULES,   SUCK_MIN_LINES, "-1 ; 100");
    SET_UNDEF_STR(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_LINES, "-1 ; 100");
    SET_UNDEF_STR(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_BYTES, "-1 ; 100");
    SET_UNDEF_STR(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_GROUPS,"-1 ; 100");
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_MSG_THRESHOLD,-1);
    SET_UNDEFINED(SUCK_GLOBAL_KILL_RULES,   SUCK_MAX_MSG_DOWNLOAD, -1);

    SET_UNDEFINED(SUCK_KILL_LOGFILE,SUCK_KILL_ENABLE_LOGFILE, false);
    SET_UNDEF_STR(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILENAME,    
        "/tmp/SuckMT-KillLog.txt");
    SET_UNDEF_STR(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILE_HEADERS,
        "From Subject Newsgroups Lines X-Trace X-Complaints-To Message-ID");

    // Keep statistics
    settings.SetValue(SUCK_COPYRIGHT,SUCK_LATEST_VERSION,string(SUCKMT_VERSION));
    settings.SetValue(SUCK_INSTALL,SUCK_LASTRUN_DATE,nowStr);
    settings.AddValue(SUCK_INSTALL,SUCK_RUN,1);

    list<string> dummy_string_list;
    
    // Check if the groups section exists
    list<string> groups;
    if (!settings.GetVariableNames(SUCK_GROUPS,dummy_string_list))
        settings.CreateSection(SUCK_GROUPS);

    // Check if the kill headers section exists
    if (!settings.GetVariableNames(SUCK_KILL_HEADERS,dummy_string_list))
    {
        // Hmmm, new to kill rules ?
        // Ok, creating one sample rule
        SET_UNDEF_STR(SUCK_KILL_HEADERS,"Content-Type:text/html","not yet ; 0 ; 10000");
        Lalways << "Created a sample kill rule in your config file." << endl << flush;
    }

    // Check if the kill headers section exists
    if (!settings.GetVariableNames(SUCK_KEEP_HEADERS,dummy_string_list))
    {
        // Hmmm, new to keep rules ?
        // Ok, creating one sample rule ... ;-)
        SET_UNDEF_STR(SUCK_KEEP_HEADERS,"~From:Niels Basjes","not yet ; 0 ; 10000");
        Lalways << "Created a sample keep rule in your config file." << endl << flush;
    }
}

//-------------------------------------------------------------------------

static NNTPRetrieveManager *retrieveManagerToSignal = NULL;

static void 
SuckmtSignalHandler(int /*sig_num*/)
{
    static int aborting = false;
    static omni_mutex abortMutex;
    abortMutex.lock();
    if (aborting)
    {
        Levent << "Received STOP signal again (ignoring)." << endl << flush;
        abortMutex.unlock();
        return;
    }

    Levent << "Received STOP signal." << endl << flush;

    aborting = true;
    
    abortMutex.unlock();
    
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
        if (!std::strcmp(argv[i],"-i"))
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
        if (!std::strcmp(argv[i],"-n"))
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
        if (!std::strcmp(argv[i],"-init"))
        {
            writeDefaultInitFile = true;
            continue;
        }

        // Be quiet ??
        if (!std::strcmp(argv[i],"-q"))
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
