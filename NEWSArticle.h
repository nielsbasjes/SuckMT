//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NEWSArticle.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:32 $
//  $Revision: 1.3 $
//  $RCSfile: NEWSArticle.h,v $
//  $Author: niels $
//=========================================================================

#ifndef NEWSARTICLE_H
#define NEWSARTICLE_H

//-------------------------------------------------------------------------

class NEWSArticle; // Forward Declaration

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include "Printable.h"

//-------------------------------------------------------------------------

#ifdef __WIN32__
#define map    std::map
#define vector std::vector
#define string std::string
#endif

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

//    // Retrieves the value of the specified header field 
//    // returns false if the header doesn't exist
//    bool
//    GetHeaderField(string headerFieldName, long &value);

    string
    GetHeader();
    
    string 
    GetBody();

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
    
    NewsArticleState    fState;
};

DEFINE_PRINTABLE_OPERATORS(NEWSArticle)

//-------------------------------------------------------------------------

#endif

// End of the file NEWSArticle.h
//=========================================================================
