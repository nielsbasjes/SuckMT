//=========================================================================
//                 Copyright (C)1999-2012 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
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

#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

//-------------------------------------------------------------------------

class CommandHandler; // Forward Declaration

//-------------------------------------------------------------------------

#include <omnithread.h>
#include "Command.h"
#include "CommandQueue.h"
#include "Abortable.h"

//-------------------------------------------------------------------------

class CommandHandler : public omni_thread , public Abortable
{
public:
    CommandHandler(CommandQueue * commandQueue);
    ~CommandHandler();

    void
    Start();

    // Add a new command to the TODO list
    // Returns true if succes, false if failed
    bool
    AddCommand(Command * newCommand);

    bool
    IsBusy();
    
    bool 
    IsRunning();
    
protected:
    CommandQueue * myCommandQueue;
    void  AbortChildren();

    virtual bool CommandHandlerCanContinue();

private:
    omni_mutex currentCommandMutex;
    Command * currentCommand;
    void* run_undetached(void* arg);
    bool  running;
    bool  busy;
};

//-------------------------------------------------------------------------

#endif // __COMMANDHANDLER_H__

// End of the file CommandHandler.h
//=========================================================================
