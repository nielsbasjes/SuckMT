//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Tokenize.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/01/08 22:26:16 $
//  $Revision: 1.8 $
//  $RCSfile: Tokenize.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "Tokenize.h"

//-------------------------------------------------------------------------

vector<string>
GetTokens(string theString, char separator)
{
    vector<string> foundStrings;
    
    string::iterator beginPos = theString.begin();
    string::iterator endPos;

    while (beginPos != theString.end())
    {
        endPos = std::find(beginPos,theString.end(),separator);
        string foundString(beginPos,endPos);
        foundStrings.push_back(foundString);
        if (endPos == theString.end())
            break;
        beginPos = ++endPos;
    }
    return foundStrings;
}

//-------------------------------------------------------------------------

// Returns true if converted
// Returns false if string was not convertable
bool 
StringToLong(const string &input, long &output)
{
    string       workinput = input;
    RemoveLeadingSpaces(workinput);
    RemoveTrailingSpaces(workinput);
    
    const char * beginptr = workinput.c_str();
    char       * endptr   = NULL;

    if (*beginptr != 0x00) 
    {   // The existing value is not an empty string
        long currentNumberValue = 
                strtol(workinput.c_str(),&endptr, 10);

        //  man strtol says: "Thus, if *nptr is not `\0' but **endptr 
        //  is `\0' on return, the entire string is valid.
        if (*endptr == 0x00 )
        {
            output = currentNumberValue;            
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------------

void 
RemoveLeadingSpaces(string &str)
{
    if (str == "")
        return;
        
    while(str.begin() != str.end() && 
          (*(str.begin()) == ' '  ||
           *(str.begin()) == '\t' ||
           *(str.begin()) == '\r' ))
        str.erase(str.begin());
}

//-------------------------------------------------------------------------

void 
RemoveTrailingSpaces(string &str)
{
    if (str == "")
        return;
        
    while(str.begin() != str.end() && 
          (*(str.end()-1) == ' '  ||
           *(str.end()-1) == '\t' ||
           *(str.end()-1) == '\r' ))
        str.erase(str.end()-1);
}

//-------------------------------------------------------------------------


// End of the file Tokenize.cpp
//=========================================================================
