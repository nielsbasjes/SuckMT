//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : SetProcTitle.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 21:31:09 $
//  $Revision: 1.2 $
//  $RCSfile: SetProcTitle.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
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

