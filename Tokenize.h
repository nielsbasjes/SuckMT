//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Tokenize.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:46 $
//  $Revision: 1.3 $
//  $RCSfile: Tokenize.h,v $
//  $Author: niels $
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

#endif// End of file

// End of the file Tokenize.h
//=========================================================================
