//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetXOVERCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:37 $
//  $Revision: 1.3 $
//  $RCSfile: NNTPGetXOVERCommand.h,v $
//  $Author: niels $
//=========================================================================

#ifndef NNTPGetXOVERCOMMAND_H
#define NNTPGetXOVERCOMMAND_H

//-------------------------------------------------------------------------

class NNTPGetXOVERCommand; // Forward Declaration

#include <string>
#include "Command.h"

//-------------------------------------------------------------------------

#ifdef __WIN32__
#define string std::string
#endif

//-------------------------------------------------------------------------

class NNTPGetXOVERCommand : public Command
{
public:
    NNTPGetXOVERCommand(string newsGroup, long startAtArticlenr = -1);
    virtual bool Execute(CommandHandler * currentHandler);
private:
    string fNewsGroup;
    long   fStartAtArticlenr;
};

//-------------------------------------------------------------------------

#endif 

// End of the file NNTPGetXOVERCommand.h
//=========================================================================
