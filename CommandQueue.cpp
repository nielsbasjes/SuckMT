/***************************************************************************
                          CommandQueue.cpp  -  description                              
                             -------------------                                         
    begin                : Wed Jul 28 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif


#include "CommandQueue.h"
#include "StatisticsKeeper.h"

Command * CommandQueue::GetCommand()
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

void CommandQueue::AddCommand(Command * newCommand)
{
    omni_mutex_lock lock(cqmutex);
    push(newCommand);
    STAT_SetValue("Command Queue Length",size());
}
