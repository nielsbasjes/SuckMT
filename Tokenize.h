//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Tokenize.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:51:39 $
//  $Revision: 1.4 $
//  $RCSfile: Tokenize.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

//-------------------------------------------------------------------------

#include <string>
#include <vector>
#include <algorithm>

#ifdef WIN32
#define string std::string
#define vector std::vector
#endif

//-------------------------------------------------------------------------

vector<string> 
GetTokens(string theString, char separator);

//-------------------------------------------------------------------------

void 
RemoveLeadingSpaces(string &str);

//-------------------------------------------------------------------------

void 
RemoveTrailingSpaces(string &str);

//-------------------------------------------------------------------------

#endif // __TOKENIZE_H__

// End of the file Tokenize.h
//=========================================================================
