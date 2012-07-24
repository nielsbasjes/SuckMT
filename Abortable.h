//=========================================================================
//                 Copyright (C)1999-2012 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
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

#ifndef __ABORTABLE_H__
#define __ABORTABLE_H__

//-------------------------------------------------------------------------

class Abortable; // Forward Declaration

//-------------------------------------------------------------------------

#include <omnithread.h>

//-------------------------------------------------------------------------

class Abortable 
{
public: 
    Abortable();
    virtual ~Abortable();
    
    void Abort(bool alsoAbortChildren = true);
    bool KeepRunning();
    virtual void AbortChildren();
    
private:
    bool keepRunning;
    omni_mutex keepRunningMutex;
};

//-------------------------------------------------------------------------

#endif // __ABORTABLE_H__

// End of the file Abortable.h
//=========================================================================
