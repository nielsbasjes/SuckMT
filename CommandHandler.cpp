//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandHandler.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/04/03 18:42:42 $
//  $Revision: 1.11 $
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
    running = false;
    busy = false;
    currentCommand = (Command*)NULL;
}

//-------------------------------------------------------------------------

CommandHandler::~CommandHandler()
{
    // Nothing to do
}

//-------------------------------------------------------------------------

bool 
CommandHandler::CommandHandlerCanContinue()
{
    return true;
}

//-------------------------------------------------------------------------

void
CommandHandler::Start()
{
    start_undetached();
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
    running = true;
    busy = false;

    currentCommandMutex.lock();
    currentCommand = myCommandQueue->GetCommand();
    currentCommandMutex.unlock();

    while (KeepRunning() && running)
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
            
            if (!CommandHandlerCanContinue())
            {
                // This instance of the command handler is broken
                // Stop using this handler and put this command 
                // back in the queue.
                currentCommandMutex.lock();
                myCommandQueue->AddCommand(currentCommand);
                currentCommand = (Command*)NULL;
                currentCommandMutex.unlock();
                running = false;
                Lerror << "Commandhandler CANNOT CONTINUE, current command was put back into the queue." << endl << flush;
            }
            else
            {            
                currentCommandMutex.lock();
                delete (currentCommand);
                if (KeepRunning())
                    currentCommand = myCommandQueue->GetCommand();
                else
                    currentCommand = (Command*)NULL;
                currentCommandMutex.unlock();
            }
            busy = false;
        }
    }
    
    currentCommandMutex.lock();
    if (currentCommand != NULL)    
        delete (currentCommand);
    currentCommandMutex.unlock();
    
    running = false;
    return NULL;
}

//-------------------------------------------------------------------------

bool 
CommandHandler::IsBusy()
{
    return busy;
}

//-------------------------------------------------------------------------

bool 
CommandHandler::IsRunning()
{
    return running;
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
