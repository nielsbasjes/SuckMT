//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPRetrieveManager.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 21:31:05 $
//  $Revision: 1.9 $
//  $RCSfile: NNTPRetrieveManager.cpp,v $
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

#include <fstream.h>
#include <iostream.h>
#include "TraceLog.h"
#include "SuckDefines.h"
#include "NNTPRetrieveManager.h"
#include "NNTPGetXOVERCommand.h"

//-------------------------------------------------------------------------

NNTPRetrieveManager::NNTPRetrieveManager (IniFile &settings)
    :fKiller(&settings)
{
    fSettings = &settings;

    // Get the groups we want.
    list<string> groups;
    fSettings->GetVariableNames(SUCK_GROUPS,groups);

    if (groups.size() == 0)
    {
        Lerror << "No groups have been defined." << endl << flush;
        return;
    }

    // Create the handlers.
    long   handlersToCreate;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_THREADS,handlersToCreate))
        handlersToCreate = 3; // Default value
    
    Linfo << "Starting " << handlersToCreate 
          << " simultaneous connections." << endl << flush;

    for (int i = 0 ; i < handlersToCreate ; i ++)
    {
        NNTPCommandHandler * newCommandHandler = new NNTPCommandHandler 
                        (this,&commands, &fKiller, fSettings);

        // Check if the connect succeeded with the first one
        // If the first one failed the the others will fail aswell.
        if (i == 0)
        {
            NNTPProxy * proxy = newCommandHandler->GetNNTPProxy();
            if (proxy == NULL ||
                (proxy != NULL && !proxy->IsConnected()))
            {
                Lerror << "Connection to server failed." << endl << flush;
                return;
            }
        }
        NNTPHandlers.push_back (newCommandHandler);
    }
    
    // Add commands for all groups.
    list<string>::const_iterator groupIter;

    for(groupIter  = groups.begin();
        groupIter != groups.end();
        groupIter ++)
    {
        long lastOneWeHave;
        if (!fSettings->GetValue(SUCK_GROUPS,(*groupIter),lastOneWeHave))
        {
            lastOneWeHave = -1;
            fSettings->SetValue(SUCK_GROUPS,(*groupIter),lastOneWeHave);
        }

        Linfo << "Checking group " << groupIter->c_str() << endl << flush;

        commands.AddCommand(new NNTPGetXOVERCommand
                          (*groupIter,lastOneWeHave+1));
    }
}

//-------------------------------------------------------------------------

NNTPRetrieveManager::~NNTPRetrieveManager()
{
    if (KeepRunning())
    {   // We were not aborted so we write the files
    
        string batchFile;
        if (!fSettings->GetValue(SUCK_CONFIG,SUCK_BATCH_FILE,batchFile))
            batchFile = "suckBatchFile"; // Default value
    
        ofstream outFile(batchFile.c_str());
        if (outFile.good())
        {
            vector<string>::iterator filesWrittenIter;

            // Send the abort signal to all handlers
            for(filesWrittenIter  = fFilesWritten.begin();
                filesWrittenIter != fFilesWritten.end();
                filesWrittenIter ++)
            {
                outFile << (*filesWrittenIter).c_str() << endl;
            }
        }        
    }

    vector<NNTPCommandHandler*>::iterator nntpHandlersIter;
    
    for(nntpHandlersIter  = NNTPHandlers.begin();
        nntpHandlersIter != NNTPHandlers.end();
        nntpHandlersIter ++)
    {
        (*nntpHandlersIter)->Abort();
        (*nntpHandlersIter)->join(NULL);
    }

    NNTPHandlers.clear();
}


//-------------------------------------------------------------------------
// Store the specified filename so we know which files have to 
// be processed later

void
NNTPRetrieveManager::ArticleFileHasBeenWritten(string fileName)
{
    omni_mutex_lock lock(nrm_mutex);
    fFilesWritten.push_back(fileName);
}


//-------------------------------------------------------------------------
// Get the messages from the specified groups
// Returns the number of downloaded messages or -1 in case of error    

void
NNTPRetrieveManager::GetGroups(vector<string> groupNames)
{
    vector<string>::iterator groupNamesIter;

    for(groupNamesIter  = groupNames.begin();
        groupNamesIter != groupNames.end();
        groupNamesIter ++)
    {
        commands.AddCommand(new NNTPGetXOVERCommand(*groupNamesIter));
    }
}

//-------------------------------------------------------------------------

void
NNTPRetrieveManager::WaitForCompletion ()
{
    vector<NNTPCommandHandler*>::iterator nntpHandlersIter;

    while (true) // Wait until the queue is empty
    {
        omni_thread::sleep(0,100000); // 100000 nanoseconds = 0.1 seconds

        // Check if any of the handlers is still busy
        for(nntpHandlersIter  = NNTPHandlers.begin();
            nntpHandlersIter != NNTPHandlers.end();
            nntpHandlersIter ++)
        {
            if ((*nntpHandlersIter)->IsBusy())
                break; // This one is still busy
        }

        // If this is true the queue is empty and none are busy
        if (commands.empty() && (nntpHandlersIter == NNTPHandlers.end()))
            break;

        // This is here because it is not possible to call the AbortChildren
        // function from the sighandler because it will lockup otherwise
        if (!KeepRunning())
            AbortChildren();
    }
}

//-------------------------------------------------------------------------

void
NNTPRetrieveManager::AbortChildren()
{
    Levent << "Making Queue EMPTY." << endl << flush;
    
    vector<NNTPCommandHandler*>::iterator nntpHandlersIter;
    
    // Send the abort signal to all handlers
    for(nntpHandlersIter  = NNTPHandlers.begin();
        nntpHandlersIter != NNTPHandlers.end();
        nntpHandlersIter ++)
    {
        (*nntpHandlersIter)->Abort();
    }

    fKiller.Abort();
        
    bool someAreBusy = true;
    
    while (someAreBusy)
    {
        someAreBusy = false;
        
        omni_thread::sleep(0,100000); // 100000 nanoseconds = 0.1 seconds

        // Wait for all the handlers to really finish
        for(nntpHandlersIter  = NNTPHandlers.begin();
            nntpHandlersIter != NNTPHandlers.end();
            nntpHandlersIter ++)
        {
            if ((*nntpHandlersIter)->IsBusy())
                someAreBusy =true; // This one is still busy
        }
    }
    
    // Remove ALL outstanding commands from the queue
    Command * command = commands.GetCommand();
    while (command != NULL)
    {
        delete (command);
        command = commands.GetCommand();
    }
}

//-------------------------------------------------------------------------

// End of the file NNTPRetrieveManager.cpp
//=========================================================================
