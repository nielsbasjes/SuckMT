//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandHandler.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 21:30:46 $
//  $Revision: 1.8 $
//  $RCSfile: CommandHandler.cpp,v $
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

#include "omnithread.h"
#include "TraceLog.h"
#include "CommandHandler.h"

//-------------------------------------------------------------------------

CommandHandler::CommandHandler(CommandQueue * commandQueue)
{
    myCommandQueue = commandQueue;
    Lcalls << "Starting Command Handler" << endl << flush;
    busy = false;
    currentCommand = (Command*)NULL;
    start_undetached();
}

//-------------------------------------------------------------------------

CommandHandler::~CommandHandler()
{
    // Nothing to do
}

//-------------------------------------------------------------------------

// Returns true if succes, false if failed
bool
CommandHandler::AddCommand(Command * newCommand)
{
    if (newCommand == NULL)
        return false;

    myCommandQueue->AddCommand(newCommand);

    return true;
}

//-------------------------------------------------------------------------

void* 
CommandHandler::run_undetached(void* /*arg*/)
{
    busy = false;

    currentCommandMutex.lock();
    currentCommand = myCommandQueue->GetCommand();
    currentCommandMutex.unlock();

    while (KeepRunning())
    {
        if(currentCommand == NULL)
        {
            omni_thread::sleep(0, 100000); // 100000 nanoseconds = 0.1 seconds
            currentCommandMutex.lock();
            currentCommand = myCommandQueue->GetCommand();
            currentCommandMutex.unlock();
        }
        else
        {
            busy = true;
            if(currentCommand->Execute(this) == false)
            {
                Lerror << "Command failed." << endl << flush;
//                busy = false;
            }

            currentCommandMutex.lock();
            delete (currentCommand);
            if (KeepRunning())
                currentCommand = myCommandQueue->GetCommand();
            else
                currentCommand = (Command*)NULL;
            currentCommandMutex.unlock();
            busy = false;
        }
    }
    
    currentCommandMutex.lock();
    if (currentCommand != NULL)    
        delete (currentCommand);
    currentCommandMutex.unlock();
    
    return NULL;
}

//-------------------------------------------------------------------------

bool 
CommandHandler::IsBusy()
{
    return busy;
}

//-------------------------------------------------------------------------

void 
CommandHandler::AbortChildren()
{
    omni_mutex_lock lock(currentCommandMutex);
    if (currentCommand != NULL)
        currentCommand->Abort();
}

//-------------------------------------------------------------------------

// End of the file CommandHandler.cpp
//=========================================================================
