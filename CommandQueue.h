//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandQueue.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2004-12-20 21:09:23 +0100 (Mon, 20 Dec 2004) $
//  $Revision: 4 $
//  $RCSfile: CommandQueue.h,v $
//  $Author: $
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

#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

//-------------------------------------------------------------------------

class CommandQueue; // Forward Declaration

//-------------------------------------------------------------------------

#include "tsqueue.h"
#include "Command.h"

//-------------------------------------------------------------------------

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
