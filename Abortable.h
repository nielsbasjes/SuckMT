//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Abortable.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:52:23 $
//  $Revision: 1.5 $
//  $RCSfile: Abortable.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __ABORTABLE_H__
#define __ABORTABLE_H__

//-------------------------------------------------------------------------

class Abortable 
{
public: 
    Abortable();
    virtual ~Abortable();
    
    void Abort();
    bool KeepRunning();
    virtual void AbortChildren();
    
private:
    bool keepRunning;
};

//-------------------------------------------------------------------------

#endif // __ABORTABLE_H__

// End of the file Abortable.h
//=========================================================================
