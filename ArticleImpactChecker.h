//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : ArticleImpactChecker.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/01/05 22:39:55 $
//  $Revision: 1.2 $
//  $RCSfile: ArticleImpactChecker.h,v $
//  $Author: niels $
//=========================================================================

#ifndef __ArticleImpactChecker_H__
#define __ArticleImpactChecker_H__
    
class ArticleImpactChecker; // Forward Declaration

//-------------------------------------------------------------------------

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

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
