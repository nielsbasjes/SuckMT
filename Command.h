//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Command.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:46:35 $
//  $Revision: 1.4 $
//  $RCSfile: Command.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __COMMAND_H__
#define __COMMAND_H__

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

#endif // __COMMAND_H__

// End of the file Command.h
//=========================================================================
