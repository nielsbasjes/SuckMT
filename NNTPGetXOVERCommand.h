/***************************************************************************
                          NNTPGetXOVERCommand.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef NNTPGetXOVERCOMMAND_H
#define NNTPGetXOVERCOMMAND_H

class NNTPGetXOVERCommand; // Forward Declaration

#include "debugging.h"
#include "Command.h"

class NNTPGetXOVERCommand : public Command
{
public:
    NNTPGetXOVERCommand(string newsGroup, long startAtArticlenr = -1);
    virtual bool Execute(CommandHandler * currentHandler);
private:
    string fNewsGroup;
    long   fStartAtArticlenr;
};

#endif




