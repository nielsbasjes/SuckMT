//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: CommandQueue.cpp 17 2008-08-17 15:29:35Z niels $
//  $Date: 2008-08-17 17:29:35 +0200 (Sun, 17 Aug 2008) $
//  $Revision: 17 $
//  $LastChangedBy: niels $
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

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "CommandQueue.h"
#include "StatisticsKeeper.h"
#include "TraceLog.h"

//-------------------------------------------------------------------------

Command * 
CommandQueue::GetCommand()
{
    omni_mutex_lock lock(cqmutex);

    if (empty())
    {
        Lsteps << "Command Queue is EMPTY" << endl << flush;
        return NULL;
    }

    Command * command = front();
    pop();

    STAT_SetValue("Command Queue Length",size());
    STAT_AddValue("Command Queue Total",1);
    return command;
}

//-------------------------------------------------------------------------

void 
CommandQueue::AddCommand(Command * newCommand)
{
    omni_mutex_lock lock(cqmutex);
    push(newCommand);
    STAT_SetValue("Command Queue Length",size());
}

//-------------------------------------------------------------------------

// End of the file CommandQueue.cpp
//=========================================================================
