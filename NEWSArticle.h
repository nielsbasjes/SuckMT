/***************************************************************************
                          NEWSArticle.h  -  description                              
                             -------------------                                         
    begin                : Sun Jul 18 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef NEWSARTICLE_H
#define NEWSARTICLE_H

class NEWSArticle; // Forward Declaration

#include "debugging.h"
#include <stdio.h>
#include <string>
#include "Printable.h"

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#endif

class NEWSArticle : public Printable
{
public: 
    NEWSArticle();
    NEWSArticle(string newsgroup, string serverLine);
    ~NEWSArticle();

    void Print (ostream &os) const;

    void SetXOVERLine(const char * line);

    bool operator== (const string& someMessageID) const;
//private:
    string          primaryNewsGroup;

    // -------------
    // XOVER fields
    unsigned long   articleNr;
    string          subject;
    string          sender;
    string          date;
    string          messageID;
    string          references;
    long            bytes;
    long            lines;
    string          xrefHeader;
    // -------------

    string          header;
    string          body;
};

DEFINE_PRINTABLE_OPERATORS(NEWSArticle)


#endif
