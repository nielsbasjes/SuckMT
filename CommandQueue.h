//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandQueue.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:46:40 $
//  $Revision: 1.3 $
//  $RCSfile: CommandQueue.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

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

#endif // __COMMANDQUEUE_H__

// End of the file CommandQueue.h
//=========================================================================
