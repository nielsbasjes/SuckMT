//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandHandler.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:20 $
//  $Revision: 1.2 $
//  $RCSfile: CommandHandler.h,v $
//  $Author: niels $
//=========================================================================

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

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

#endif // COMMANDHANDLER_H

// End of the file CommandHandler.h
//=========================================================================
