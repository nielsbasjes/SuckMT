//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetXOVERCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/10/07 19:43:14 $
//  $Revision: 1.4 $
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

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
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
