//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Abortable.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/05/05 20:03:17 $
//  $Revision: 1.7 $
//  $RCSfile: Abortable.h,v $
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
