//=========================================================================
//                 Copyright (C)1999-2003 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT/
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : SetProcTitle.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2003/04/13 20:51:55 $
//  $Revision: 1.5 $
//  $RCSfile: SetProcTitle.h,v $
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

#ifndef __SETPROCTITLE_H__
#define __SETPROCTITLE_H__

//-------------------------------------------------------------------------

void
InitSetProcTitle(
	int argc,
	char **argv,
	char **envp,
    const char * applicationPrefix);

void
SetProcTitle(const char *fmt, ...);

//-------------------------------------------------------------------------

#endif // __SETPROCTITLE_H__

// End of the file SetProcTitle.h
//=========================================================================


