//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/30 17:35:00 $
//  $Revision: 1.4 $
//  $RCSfile: NewsKiller.h,v $
//  $Author: niels $
//=========================================================================

#ifndef NEWSKILLER_H
#define NEWSKILLER_H

//-------------------------------------------------------------------------

class NewsKiller; // Forward declaration

#include <map>
#include <string>
#include "NEWSArticle.h"
#include "IniFile.h"
#include "Printable.h"
#include "omnithread.h"

//-------------------------------------------------------------------------

#ifdef __WIN32__
#define map    std::map
#define string std::string
#endif

//-------------------------------------------------------------------------

class NewsKiller : public Printable
{
public: 
	NewsKiller(IniFile *settings);
	~NewsKiller();

    bool 
    DoWeKeepThisArticle(NEWSArticle * article);

    void Print (ostream &os) const;

private:
    //------------------------------------------------------
    // Private members that are actually used for the storage 
    omni_mutex valuesMutex;    

    // Returns 
    //  - true  if stored.
    //  - false if not stored because it is already present.
    bool
    StoreMessageID(string newMessageID);

    // The string is the messageID and 
    // the bool is always true except when not yet present
    map <string,bool> allMessageIDs;
    omni_mutex       messageDBmutex;
    
    // The settings file contains all the kill rules
    IniFile          *fSettings;
    
    // The current time and date as a string
    string            fNow;
    
    typedef struct 
    {
        string  keyName;
        string  valueToKill;
        string  lastOcurrance;
        long    count;
    } killStruct;
        
    // Pre parsed cache for the kill rules
    vector<string>                  fHeadersToCheck;
    map<string,vector<killStruct*> > fKillHeaders;
    
    // The headername is the name of the header that is passed
    // The headerValue is the value of the header that is passed
    // The return value is false if this article should be killed
    bool 
    DoWeKeepThisArticle(string headername,string headerValue);
    
};

DEFINE_PRINTABLE_OPERATORS(NewsKiller)

//-------------------------------------------------------------------------

#endif

// End of the file NewsKiller.h
//=========================================================================
