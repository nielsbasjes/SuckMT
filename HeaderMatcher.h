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

#ifndef __HeaderMatcher_H__
#define __HeaderMatcher_H__
    
//-------------------------------------------------------------------------

class HeaderMatcher; // Forward Declaration

//-------------------------------------------------------------------------

#include <strstream>

#include "boost/regex.hpp"
using namespace boost;

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
    regex*   fRegExpression;
};

DEFINE_PRINTABLE_OPERATORS(HeaderMatcher)

//-------------------------------------------------------------------------

#endif // __HEADERCHECKER_H__

// End of the file HeaderMatcher.h
//=========================================================================
