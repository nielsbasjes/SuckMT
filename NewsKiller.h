//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/05/05 20:03:13 $
//  $Revision: 1.14 $
//  $RCSfile: NewsKiller.h,v $
//  $Author: niels $
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
#include <omnithread.h>
#include "HeaderMatcher.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define map    std::map
#define vector std::vector
#define string std::string
#endif

//-------------------------------------------------------------------------

class NewsKiller
{
public: 
    NewsKiller(IniFile *settings);
    ~NewsKiller();

    bool 
    DoWeKeepThisArticle(NEWSArticle * article);

private:
    //------------------------------------------------------
    // Private members that are actually used for the storage 
    omni_mutex valuesMutex;    

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

//-------------------------------------------------------------------------

#endif // __NEWSKILLER_H__

// End of the file NewsKiller.h
//=========================================================================
