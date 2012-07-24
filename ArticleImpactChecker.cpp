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

#ifdef _MSC_VER
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
