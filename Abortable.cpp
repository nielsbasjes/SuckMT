//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Abortable.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:10 $
//  $Revision: 1.2 $
//  $RCSfile: Abortable.cpp,v $
//  $Author: niels $
//=========================================================================


#ifdef WIN32
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
Abortable::Abort()
{
    if (keepRunning)
    {
        keepRunning = false;
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
