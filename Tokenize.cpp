//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : Tokenize.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/10/07 19:45:52 $
//  $Revision: 1.4 $
//  $RCSfile: Tokenize.cpp,v $
//  $Author: niels $
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
