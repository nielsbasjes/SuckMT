//=========================================================================
//                 Copyright (C)1999-2003 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT/
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetXOVERCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2003/04/13 20:51:55 $
//  $Revision: 1.9 $
//  $RCSfile: NNTPGetXOVERCommand.h,v $
//  $Author: niels $
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

#ifndef __NNTPGETXOVERCOMMAND_H__
#define __NNTPGETXOVERCOMMAND_H__

//-------------------------------------------------------------------------

class NNTPGetXOVERCommand; // Forward Declaration

#include <string>
#include "Command.h"

//-------------------------------------------------------------------------

using namespace std;

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

#endif // __NNTPGETXOVERCOMMAND_H__

// End of the file NNTPGetXOVERCommand.h
//=========================================================================
