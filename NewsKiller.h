//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/10/07 19:43:12 $
//  $Revision: 1.7 $
//  $RCSfile: NewsKiller.h,v $
//  $Author: niels $
//=========================================================================

#ifndef NEWSKILLER_H
#define NEWSKILLER_H

//-------------------------------------------------------------------------

class NewsKiller; // Forward declaration

//-------------------------------------------------------------------------

#include <map>
#include <vector>
#include <string>

#ifdef WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <fstream.h>
#include "NEWSArticle.h"
#include "IniFile.h"
#include "Printable.h"
#include "omnithread.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define map    std::map
#define vector std::vector
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

    // Kill Logfile    
    bool              fLogKilledMessages;
    string            fKillLogFileName;
    vector<string>    fHeadersToMentionInKillLog;
    ofstream          fKillLogFile;
    omni_mutex        fKillLogFileMutex;
    
    void 
    LogKillEvent(NEWSArticle *article,
                 const char * reason);
    
    typedef struct 
    {
        string  keyName;
        string  valueToMatch;
        string  lastOcurrance;
        long    count;
    } headerMatchStruct;
        
    // Pre parsed cache for the kill/keep rules
    vector<string>                          fHeadersToCheck;
    map<string,vector<headerMatchStruct*> > fKillHeaders;
    map<string,vector<headerMatchStruct*> > fKeepHeaders;

    // This function reads the rules from the Inifile and stores 
    // them in the specified storage
    void
    ReadHeaderRules (string iniSection, vector<string>  &headersToCheck,
                     map<string,vector<headerMatchStruct*> >    &headerRules);

    void
    EraseHeaderRules(map<string,vector<headerMatchStruct*> >    &headerRules);

    void
    CheckLinesAndBytesAndGroups(NEWSArticle * article, strstream &killReasons);

    void
    MatchHeaders(NEWSArticle * article, strstream &killReasons);

    void 
    MatchOneHeader(NEWSArticle * article, strstream &killReasons, string headerName);

    // The rule set is either the Kill (true) or the Keep (false) set
    // The headername is the name of the header that is passed
    // The headerValue is the value of the header that is passed
    // The return value is true if this article should be killed
    bool 
    DoesTheHeaderMatch(bool   kill_keep,
                       string headerName,
                       string headerValue,
                       string &matchedValue);
    #define USE_KILL  true
    #define USE_KEEP  false
};

DEFINE_PRINTABLE_OPERATORS(NewsKiller)

//-------------------------------------------------------------------------

#endif

//-------------------------------------------------------------------------

// End of the file NewsKiller.h
//=========================================================================
