//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Abortable.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: Abortable.cpp 5 2004-12-20 20:20:02Z niels $
//  $Date: 2004-12-20 21:20:02 +0100 (Mon, 20 Dec 2004) $
//  $Revision: 5 $
//  $LastChangedBy: niels $
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
