//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandHandler.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:46:38 $
//  $Revision: 1.3 $
//  $RCSfile: CommandHandler.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __COMMANDHANDLER_H__
#define __COMMANDHANDLER_H__

//-------------------------------------------------------------------------

class CommandHandler; // Forward Declaration

#include "omnithread.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Abortable.h"

//-------------------------------------------------------------------------

class CommandHandler : public omni_thread , public Abortable
{
public:
    CommandHandler(CommandQueue * commandQueue);
    ~CommandHandler();

    // Add a new command to the TODO list
    // Returns true if succes, false if failed
    bool
    AddCommand(Command * newCommand);

    bool
    IsBusy();
    
protected:
    CommandQueue * myCommandQueue;

private:
    omni_mutex currentCommandMutex;
    Command * currentCommand;
    void* run_undetached(void* arg);
    bool  busy;
    void  AbortChildren();
};

//-------------------------------------------------------------------------

#endif // __COMMANDHANDLER_H__

// End of the file CommandHandler.h
//=========================================================================
