//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : ArticleImpactChecker.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2004-12-20 21:09:23 +0100 (Mon, 20 Dec 2004) $
//  $Revision: 4 $
//  $RCSfile: ArticleImpactChecker.h,v $
//  $Author: $
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

#ifndef __ArticleImpactChecker_H__
#define __ArticleImpactChecker_H__
    
//-------------------------------------------------------------------------

class ArticleImpactChecker; // Forward Declaration

//-------------------------------------------------------------------------

#include <strstream>

#include "IniFile.h"
#include "Printable.h"
#include "NEWSArticle.h"

//-------------------------------------------------------------------------
// This class reads the specified value from the specified IniFile
// and logs this event by itself when a matching query is performed.

class ArticleImpactChecker
{
public: 
    ArticleImpactChecker(IniFile *settings);

    ArticleImpactChecker(const ArticleImpactChecker &original);

    virtual ~ArticleImpactChecker();

    virtual long
    GetImpactValue(
        NEWSArticle * article,
        const string &headerName,
        string &matchlog) = 0;

    // This function returns the name of the header
    // for which this ArticleImpactChecker will check.
    // Returns true if headerName is filled.
    // Returns false if this ArticleImpactChecker will check
    // Several headers
    virtual bool
    GetMatchingHeader(string &headerName);

protected:
    IniFile *fSettings;
    string   fNow;
};

DEFINE_PRINTABLE_OPERATORS(ArticleImpactChecker)

//-------------------------------------------------------------------------

#endif // __ArticleImpactChecker_H__

// End of the file ArticleImpactChecker.h
//=========================================================================
