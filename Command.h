//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Command.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:23 $
//  $Revision: 1.3 $
//  $RCSfile: Command.h,v $
//  $Author: niels $
//=========================================================================

#ifndef COMMAND_H
#define COMMAND_H

//-------------------------------------------------------------------------

class Command; // Forward Declaration

#include "CommandHandler.h"
#include "Abortable.h"

//-------------------------------------------------------------------------

class Command : public Abortable
{
public:
    Command();
    virtual ~Command();
    virtual bool Execute(CommandHandler * currentHandler) = 0;
};

//-------------------------------------------------------------------------

#endif

// End of the file Command.h
//=========================================================================
