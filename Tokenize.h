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

#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

//-------------------------------------------------------------------------

#include <string>
#include <vector>
#include <algorithm>

//-------------------------------------------------------------------------

using namespace std;

//-------------------------------------------------------------------------

vector<string> 
GetTokens(string theString, char separator);

//-------------------------------------------------------------------------
// Returns true if converted
// Returns false if string was not convertable
bool 
StringToLong(const string &input, long &output);

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
