//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Abortable.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2002/02/18 23:08:42 $
//  $Revision: 1.7 $
//  $RCSfile: Abortable.cpp,v $
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

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "Abortable.h"

//-------------------------------------------------------------------------

Abortable::Abortable()
{
    keepRunning = true;
}

//-------------------------------------------------------------------------

Abortable::~Abortable()
{
    // Nothing to to
}

//-------------------------------------------------------------------------

void
Abortable::Abort(bool alsoAbortChildren)
{
    omni_mutex_lock lock(keepRunningMutex);
    if (keepRunning)
    {
        keepRunning = false;
        if (alsoAbortChildren)
            AbortChildren();
    }
}

//-------------------------------------------------------------------------

bool
Abortable::KeepRunning()
{
    return keepRunning;
}

//-------------------------------------------------------------------------

void
Abortable::AbortChildren()
{
    // Nothing to to
}

//-------------------------------------------------------------------------

// End of the file Abortable.cpp
//=========================================================================
