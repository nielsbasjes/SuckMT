//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/02 20:51:30 $
//  $Revision: 1.12 $
//  $RCSfile: NewsKiller.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __NEWSKILLER_H__
#define __NEWSKILLER_H__

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
#include "HeaderMatcher.h"
#include "DuplicatesChecker.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define map    std::map
#define vector std::vector
#define string std::string
#endif

//-------------------------------------------------------------------------

class NewsKiller : public Printable, public Abortable
{
public: 
    NewsKiller(IniFile *settings);
    ~NewsKiller();

    bool 
    DoWeKeepThisArticle(NEWSArticle * article);

    void 
    AbortChildren();

    void 
    Print (ostream &os) const;

private:
    //------------------------------------------------------
    // Private members that are actually used for the storage 
    omni_mutex valuesMutex;    

    // The settings file contains all downloaded message IDs
    DuplicatesChecker fDuplicatesChecker;

    // The settings file contains all the kill rules
    IniFile          *fSettings;
    
    // The current time and date as a string
    string            fNow;

    //----------    
    // Kill Logfile    
    bool              fLogKilledMessages;
    string            fKillLogFileName;
    vector<string>    fHeadersToMentionInKillLog;
    ofstream          fKillLogFile;
    omni_mutex        fKillLogFileMutex;

    void
    InitializeLogFile();
        
    //----------    
    long    fMinLines;
    long    fMinLinesImpact;
    long    fMaxLines;
    long    fMaxLinesImpact;
    long    fMaxBytes;
    long    fMaxBytesImpact;
    long    fMaxGroups;
    long    fMaxGroupsImpact;

    void 
    ReadGlobalKillRule(string valueName,long &value, long &valueImpact);
    void
    ReadLinesBytesGroupsRules();
    
    //----------    
    void 
    LogKillEvent(NEWSArticle *article,
                 const char * reason);
    
    // Pre parsed cache for the kill/keep rules
    vector<string>                      fHeadersToCheck;
    map<string,vector<HeaderMatcher*> > fKillHeaders;
    vector<HeaderMatcher*>              fKillOther;
    map<string,vector<HeaderMatcher*> > fKeepHeaders;
    vector<HeaderMatcher*>              fKeepOther;

    // This function reads the rules from the Inifile and stores 
    // them in the specified storage
    void
    ReadHeaderRules (string iniSection, vector<string>  &headersToCheck,
                     map<string,vector<HeaderMatcher*> > &headerRules,
                     vector<HeaderMatcher*> &otherRules);

    void
    EraseHeaderRules(map<string,vector<HeaderMatcher*> >    &headerRules);

    void
    EraseHeaderRules(vector<HeaderMatcher*> &headerRules);

    void
    CheckLinesAndBytesAndGroups(NEWSArticle * article, strstream &killReasons);

    void
    ExecuteHeaderMatchers(NEWSArticle * article, strstream &killReasons);

    void 
    ExecuteHeaderMatchers(NEWSArticle * article, strstream &killReasons, 
                          string headerName);
};

DEFINE_PRINTABLE_OPERATORS(NewsKiller)

//-------------------------------------------------------------------------

#endif // __NEWSKILLER_H__

// End of the file NewsKiller.h
//=========================================================================
