//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPGetXOVERCommand.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/05/05 20:03:13 $
//  $Revision: 1.6 $
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

#endif // __NNTPGETXOVERCOMMAND_H__

// End of the file NNTPGetXOVERCommand.h
//=========================================================================
