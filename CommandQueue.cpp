//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : CommandQueue.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:22 $
//  $Revision: 1.2 $
//  $RCSfile: CommandQueue.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "CommandQueue.h"
#include "StatisticsKeeper.h"

//-------------------------------------------------------------------------

Command * 
CommandQueue::GetCommand()
{
    omni_mutex_lock lock(cqmutex);

    if (empty())
    {
        //cout << "Command Queue is EMPTY" << endl << flush;
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
