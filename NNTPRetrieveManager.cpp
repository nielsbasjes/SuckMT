//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPRetrieveManager.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/28 20:06:24 $
//  $Revision: 1.11 $
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
    ,fDuplicatesChecker(&settings)
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

    AddGroupsToCommandQueue(groups);
    if (!CreateHandlers())
        return;
}

//-------------------------------------------------------------------------

bool
NNTPRetrieveManager::CreateHandlers()
{
    // Create the handlers.
    long   handlersToCreate;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_THREADS,handlersToCreate))
        handlersToCreate = 3; // Default value

    // Get the maximum number of connect retries.
    long   maxNumberOfRetries;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_CONNECT_RETRIES,maxNumberOfRetries))
        maxNumberOfRetries = 3; // Default value

    // You want more than 10 retries ???? .... sure you do.
    if (maxNumberOfRetries < 0 || maxNumberOfRetries > 10)
    {
        maxNumberOfRetries = 3; // Default value
    }
    
    Linfo << "Starting " << handlersToCreate 
          << " simultaneous connections." << endl << flush;

    for (int handlerNr = 0 ; handlerNr < handlersToCreate ; handlerNr ++)
    {
        NNTPCommandHandler * newCommandHandler = NULL;
        
        // Ok, this is a bit creative.
        // Sometimes the internet provider refuses every other connection
        // So for each connection we want we do a number of retries.
        bool handlerOk = false;
        int  attempt = 0; // We start with attempt 0
        while (!handlerOk && (attempt <= maxNumberOfRetries ))
        {
            attempt++;

            // Create the connection object.
            newCommandHandler = new NNTPCommandHandler 
                        (this,&commands, &fKiller, fSettings);

            // Check if the connect succeeded
            NNTPProxy * proxy = newCommandHandler->GetNNTPProxy();
            if (proxy == NULL ||
                (proxy != NULL && !proxy->IsConnected()))
            {
                // Connection failed
                delete newCommandHandler;
                Lerror << "Connection to server failed (attempt "
                       << attempt << " for handler " << handlerNr 
                       << ")." << endl << flush;
            }
            else
            {
                handlerOk = true; // Connected
            }
        }

        if (!handlerOk)
        {
            Lerror << "Connecting to server failed completely after "
                   << attempt << " attempts." << endl << flush;
            return false;
        }

        // Now we start the handler
        newCommandHandler->Start();
        NNTPHandlers.push_back (newCommandHandler);
    }
    return true;
}

//-------------------------------------------------------------------------

void
NNTPRetrieveManager::AddGroupsToCommandQueue(list<string> &groups)
{
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
// Store the specified messageID so we can check if we get any duplicates
// Returns 
//  - true  if the messageID was unknown and it has been stored.
//  - false if the messageID was already present and it has not been stored.

bool
NNTPRetrieveManager::DoWeNeedToDownloadThisArticle(NEWSArticle * article)
{
    return fDuplicatesChecker.DoWeNeedToDownloadThisArticle(article);
}

//-------------------------------------------------------------------------
// Make a note this article has been stored in the specified file

void
NNTPRetrieveManager::ArticleHasBeenStored(NEWSArticle * article,string filename)
{
    omni_mutex_lock lock(nrm_mutex);
    fFilesWritten.push_back(filename);
    fDuplicatesChecker.ArticleHasBeenStored(article);
}

//-------------------------------------------------------------------------
// Make a note this article has been stored

void
NNTPRetrieveManager::ArticleHasBeenKilled(NEWSArticle * article)
{
    fDuplicatesChecker.ArticleHasBeenKilled(article);
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

    while (true) // Wait until the queue is empty or all handlers have died
    {
        omni_thread::sleep(0,100000); // 100000 nanoseconds = 0.1 seconds

        // ----------    
        // Check if any of the handlers is still busy
        for(nntpHandlersIter  = NNTPHandlers.begin();
            nntpHandlersIter != NNTPHandlers.end();
            nntpHandlersIter ++)
        {
            if ((*nntpHandlersIter)->IsBusy())
                break; // This atleast one is still busy
        }

        // If this is true the queue is empty and none are busy
        if (commands.empty() && (nntpHandlersIter == NNTPHandlers.end()))
            break;

        // ----------    
        // This is here because it is not possible to call the AbortChildren
        // function from the sighandler because it will lockup otherwise
        if (!KeepRunning())
            AbortChildren();
        
        // ----------    
        // Check if some of the handlers are still running
        for(nntpHandlersIter  = NNTPHandlers.begin();
            nntpHandlersIter != NNTPHandlers.end();
            nntpHandlersIter ++)
        {
            if ((*nntpHandlersIter)->IsRunning())
                break; // There is atleast one still running
        }
        
        // If this is true none are left running
        if (KeepRunning() && nntpHandlersIter == NNTPHandlers.end())
        {
            Linfo << "All connections died unexpectedly." << endl << flush;
            Abort(); // Broadcast the decision to abort.
            break;
        }
            
        // ----------    
        // This is here because it is not possible to call the AbortChildren
        // function from the sighandler because it will lockup otherwise
        if (!KeepRunning())
            AbortChildren();

        // ----------    
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

    fDuplicatesChecker.Abort();
        
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
            if ((*nntpHandlersIter)->IsRunning())
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
