/***************************************************************************
                          Command.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef COMMAND_H
#define COMMAND_H

class Command; // Forward Declaration

#include "debugging.h"
#include "CommandHandler.h"
#include "Abortable.h"

class Command : public Abortable
{
public:
    Command();
    virtual ~Command();
    virtual bool Execute(CommandHandler * currentHandler) = 0;
};

#endif




