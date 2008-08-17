//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: NEWSArticle.h 17 2008-08-17 15:29:35Z niels $
//  $Date: 2008-08-17 17:29:35 +0200 (Sun, 17 Aug 2008) $
//  $Revision: 17 $
//  $LastChangedBy: niels $
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

#ifndef __NEWSARTICLE_H__
#define __NEWSARTICLE_H__

//-------------------------------------------------------------------------

class NEWSArticle; // Forward Declaration

//-------------------------------------------------------------------------

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include "Printable.h"

//-------------------------------------------------------------------------

using namespace std;

//-------------------------------------------------------------------------

typedef enum { NA_NOTHING, NA_ONLY_XOVER, NA_ONLY_HEADER, NA_COMPLETE }
NewsArticleState;

class NEWSArticle : public Printable
{
public: 
    NEWSArticle();
    NEWSArticle(string newsgroup, string serverLine);
    ~NEWSArticle();

    void Print (ostream &os) const;

    void SetXOVERLine(const char * line);
    bool StoreHeader(const string &line);
    bool StoreBody(const string &line);

    NewsArticleState GetState();

    // Retrieves the value of the specified header field 
    // returns false if the header doesn't exist
    bool
    GetHeaderField(string headerName, string &value);

    string
    GetHeader();
    
    string 
    GetBody();

    long 
    Score(long additionalScore = 0);

    bool operator== (const string& someMessageID) const;
//private:
    string          fPrimaryNewsGroup;

    // -------------
    // XOVER fields
    unsigned long   fArticleNr;
    string          fSubject;
    string          fSender;
    string          fDate;
    string          fMessageID;
    string          fReferences;
    vector<string>  fParsedReferences;
    unsigned long   fBytes;
    unsigned long   fLines;
    string          fXrefHeader;    
    // -------------
    unsigned long   fNrOfNewsGroups;

private:
    vector<string>      fParsedHeaderFields;
    map<string,string>  fParsedHeaders;
    vector<string>      fNewsGroups;

    string              fHeader;
    string              fBody;

    bool                fKillMatched;
    bool                fKeepMatched;
    long                fKillKeepScore;

    NewsArticleState    fState;
};

DEFINE_PRINTABLE_OPERATORS(NEWSArticle)

//-------------------------------------------------------------------------

#endif // __NEWSARTICLE_H__

// End of the file NEWSArticle.h
//=========================================================================
