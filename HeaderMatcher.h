//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : HeaderMatcher.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/01/05 22:30:06 $
//  $Revision: 1.3 $
//  $RCSfile: HeaderMatcher.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __HeaderMatcher_H__
#define __HeaderMatcher_H__
    
class HeaderMatcher; // Forward Declaration

//-------------------------------------------------------------------------

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include "regex.h"
#include "IniFile.h"
#include "Printable.h"
#include "ArticleImpactChecker.h"

//-------------------------------------------------------------------------
// This class reads the specified value from the specified IniFile
// and logs this event by itself when a matching query is performed.

class HeaderMatcher: public Printable, public ArticleImpactChecker
{
public: 
    HeaderMatcher(IniFile *settings, 
                  const string sectionName, 
                  const string valueName);

//    HeaderMatcher(const HeaderMatcher &original);

    ~HeaderMatcher();

    virtual long
    GetImpactValue(
        NEWSArticle * article,
        const string &headerName,
        string &matchlog);

    bool
    GetMatchingHeader(string & headerName);

    void Print (ostream &os) const;
        
private:
    omni_mutex valuesMutex;    
    
    // Object validity checker
    bool     fObjectIsValid;
    // Raw Storage system
    string   fIniSectionName;
    string   fIniValueName;
    string   fIniValue;

    // Cooked values that have been extracted
    string   fHeaderName;
    string   fHeaderValue;
    string   fLastMatchDate;
    long     fMatchCount;
    long     fImpactValue;

    bool     fSearchCaseINSensitive;
	regex_t  fRegExpression;
};

DEFINE_PRINTABLE_OPERATORS(HeaderMatcher)

//-------------------------------------------------------------------------

#endif // __HEADERCHECKER_H__

// End of the file HeaderMatcher.h
//=========================================================================
