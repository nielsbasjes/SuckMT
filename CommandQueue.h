//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandQueue.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:23 $
//  $Revision: 1.2 $
//  $RCSfile: CommandQueue.h,v $
//  $Author: niels $
//=========================================================================

#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

//-------------------------------------------------------------------------

class CommandQueue; // Forward Declaration

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

#endif

// End of the file CommandQueue.h
//=========================================================================
