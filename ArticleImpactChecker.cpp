//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : ArticleImpactChecker.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/01/02 22:33:00 $
//  $Revision: 1.2 $
//  $RCSfile: ArticleImpactChecker.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "ArticleImpactChecker.h"
#include <time.h>

//-------------------------------------------------------------------------

ArticleImpactChecker::ArticleImpactChecker(IniFile *settings)
{
    fSettings = settings;

    // ----------
    // Get the current time
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string

    fNow = nowStr;
}

//-------------------------------------------------------------------------

ArticleImpactChecker::ArticleImpactChecker(const ArticleImpactChecker &original)
{
    fSettings = original.fSettings;
    fNow      = original.fNow;
}

//-------------------------------------------------------------------------

ArticleImpactChecker::~ArticleImpactChecker()
{
    // Nothing to do
}

//-------------------------------------------------------------------------

// This function returns the name of the header
// for which this ArticleImpactChecker will check.
// Returns true if headerName is filled.
// Returns false if this ArticleImpactChecker will check
// Several headers
bool
ArticleImpactChecker::GetMatchingHeader(string & /*headerName*/)
{
    return false;
}

//-------------------------------------------------------------------------

// End of the file ArticleImpactChecker.cpp
//=========================================================================
