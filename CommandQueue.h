/***************************************************************************
                          CommandQueue.h  -  description                              
                             -------------------                                         
    begin                : Wed Jul 28 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

class CommandQueue; // Forward Declaration

#include "tsqueue.h"
#include "Command.h"

class CommandQueue : public thread_safe_queue<Command *>
{
public:
    Command * GetCommand();
    void AddCommand(Command * newCommand);

private:
    omni_mutex cqmutex;
};

#endif
