/***************************************************************************
                          CommandHandler.cpp  -  description                              
                             -------------------                                         
    begin                : Wed Jul 28 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

class CommandHandler; // Forward Declaration

#include "omnithread.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Abortable.h"

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

#endif // COMMANDHANDLER_H
