//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Abortable.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:12 $
//  $Revision: 1.2 $
//  $RCSfile: Abortable.h,v $
//  $Author: niels $
//=========================================================================

#ifndef ABORTABLE_H
#define ABORTABLE_H

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

#endif

// End of the file Abortable.h
//=========================================================================
