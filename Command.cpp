//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Command.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:46:34 $
//  $Revision: 1.3 $
//  $RCSfile: Command.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include <iostream.h>
#include "Command.h"

//-------------------------------------------------------------------------

Command::Command()
{
//cout << "+" << flush;
    // Nothing to do
}

//-------------------------------------------------------------------------

Command::~Command()
{
//cout << "-" << flush;
    // Nothing to do
}

//-------------------------------------------------------------------------

// End of the file Command.cpp
//=========================================================================
