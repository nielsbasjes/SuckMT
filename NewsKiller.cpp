//=========================================================================
//                   Copyright (C) 2000 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/01/06 20:25:53 $
//  $Revision: 1.15 $
//  $RCSfile: NewsKiller.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include <algorithm>
#include <time.h>
#include "omnithread.h"
#include "SuckDefines.h"
#include "NewsKiller.h"
#include "Tokenize.h"
#include "StatisticsKeeper.h"

//-------------------------------------------------------------------------

NewsKiller::NewsKiller(IniFile * settings)
{
    omni_mutex_lock lock(fKillLogFileMutex);
    
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string

    fNow = nowStr;

    fSettings = settings;

    fLogKilledMessages = false; // Default value

    if (fSettings != NULL)
    {
        InitializeLogFile();
        ReadLinesBytesGroupsRules();
        ReadHeaderRules(SUCK_KILL_HEADERS,fHeadersToCheck,fKillHeaders,fKillOther);
        ReadHeaderRules(SUCK_KEEP_HEADERS,fHeadersToCheck,fKeepHeaders,fKeepOther);
    }
    
//cout << this;
    
}

//-------------------------------------------------------------------------

void
NewsKiller::InitializeLogFile()
{
    if (fSettings->GetValue(SUCK_KILL_LOGFILE,SUCK_KILL_ENABLE_LOGFILE,fLogKilledMessages))
    {
        if (fLogKilledMessages)
        {
            if (!fSettings->GetValue(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILENAME, fKillLogFileName))
            {
                fKillLogFileName = "/tmp/SuckMTKillLog.txt";
            }

            string headersToLog;

            if (!fSettings->GetValue(SUCK_KILL_LOGFILE,SUCK_KILL_LOGFILE_HEADERS, headersToLog))
            {
                headersToLog = "From Subject Newsgroups Lines X-Trace X-Complaints-To Message-ID";
            }

            fHeadersToMentionInKillLog = GetTokens(headersToLog,' ');

            fKillLogFile.open(fKillLogFileName.c_str(),ofstream::app); // Open the logfile in append mode

            if (!fKillLogFile.is_open())
            {
                cout << "Unable to open the file \"" << fKillLogFileName 
                     << "\" for appending the kill statistics log file." << endl << flush;
                fLogKilledMessages = false;
            }                
            else
            {
                // Place header in the logfile
                fKillLogFile 
                     << "====================================================" << endl
                     << "Suck MT "<< SUCKMT_VERSION <<" - A Multi Threaded suck replacement." << endl
                     << "Run started at : " << fNow << endl << flush;
            }        
        }
    }
}

//-------------------------------------------------------------------------

void
NewsKiller::ReadGlobalKillRule(string valueName,long &value, long &valueImpact)
{
    string iniLine;
    vector<string> iniLineList;

    // ==========
    value       = -1;
    valueImpact = 100;
    fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,
                        valueName,
                        iniLine);

    // Split the value into the required parts
    iniLineList = GetTokens(iniLine,';');
    switch (iniLineList.size())
    {
        case 2: StringToLong(iniLineList[1],valueImpact);
        case 1: StringToLong(iniLineList[0],value);
    }

    char valuestr[50];
    sprintf(valuestr,"%5ld ; %5ld",value,valueImpact);
    fSettings->SetValue(SUCK_GLOBAL_KILL_RULES, valueName, valuestr);
}

//-------------------------------------------------------------------------

void
NewsKiller::ReadLinesBytesGroupsRules()
{
    ReadGlobalKillRule(SUCK_MIN_LINES, fMinLines, fMinLinesImpact);
    ReadGlobalKillRule(SUCK_MAX_LINES, fMaxLines, fMaxLinesImpact);
    ReadGlobalKillRule(SUCK_MAX_BYTES, fMaxBytes, fMaxBytesImpact);
    ReadGlobalKillRule(SUCK_MAX_GROUPS,fMaxGroups,fMaxGroupsImpact);

    if (fMinLines > 0 && fMaxLines > 0 && fMinLines >= fMaxLines)
    {
        cerr << "The specified minimum number of lines in a message "
             << "is LARGER than the maximum." << endl;
    }
}

//-------------------------------------------------------------------------

void
NewsKiller::ReadHeaderRules
    (string                               iniSection,
     vector<string>                      &headersToCheck,
     map<string,vector<HeaderMatcher*> > &headerRules,
     vector<HeaderMatcher*>              &otherRules)
{   
    list<string> variableNames;
    if (fSettings->GetVariableNames(iniSection,variableNames))
    {
        for(list<string>::const_iterator
            itemIter  = variableNames.begin();
            itemIter != variableNames.end();
            itemIter ++)
        {
            string variableName = (*itemIter);
            HeaderMatcher * rule = new HeaderMatcher(fSettings,iniSection,variableName);

            string fieldName;
            if (rule->GetMatchingHeader(fieldName))
            {
                // Store the field kill rule
                if (headerRules.find(fieldName) == headerRules.end() &&
                    std::find(headersToCheck.begin(),
                              headersToCheck.end(),
                              fieldName) == headersToCheck.end())
                {   // This is a new field
                    headersToCheck.push_back(fieldName);
                }

                headerRules[fieldName].push_back(rule);
            }
            else
            {
                otherRules.push_back(rule);
            }
        }
    }
}

//-------------------------------------------------------------------------

NewsKiller::~NewsKiller()
{
    EraseHeaderRules(fKillHeaders);
    EraseHeaderRules(fKillOther);
    EraseHeaderRules(fKeepHeaders);
    EraseHeaderRules(fKeepOther);
    
    fKillLogFile 
        << "----------------------------------------------------" << endl
        << "End of run started at : " << fNow << endl << flush
        << "====================================================" << endl << flush;
}

//-------------------------------------------------------------------------

void
NewsKiller::EraseHeaderRules(map<string,vector<HeaderMatcher*> > &headerRules)
{
    for(map<string,vector<HeaderMatcher*> >::iterator
        mapIter  = headerRules.begin();
        mapIter != headerRules.end();
        mapIter ++)
    {
        EraseHeaderRules(mapIter->second);
    }
}

//-------------------------------------------------------------------------

void
NewsKiller::EraseHeaderRules(vector<HeaderMatcher*> &headerRules)
{
    for(vector<HeaderMatcher*>::iterator
        headerIter  = headerRules.begin();
        headerIter != headerRules.end();
        headerIter ++)
    {
        delete *headerIter;
    }
}

//-------------------------------------------------------------------------
// The main routine of the Kill processor.
// Returns 
//  - true  if we should continue with this article.
//  - false if we should kill this article.
bool 
NewsKiller::DoWeKeepThisArticle(NEWSArticle * article)
{
    strstream killReasons; 
    
    string matchedValue;

    if (article == NULL)
        return false;

    switch (article->GetState())
    {
        // ----------------
        case NA_NOTHING:
            break; // There is nothing to check yet

        // ----------------
        case NA_ONLY_XOVER:
            {
                // Check if this article isn't already downloaded from
                // an other newsgroup
                //=========================================
                if (!StoreMessageID(article->fMessageID))
                {
                    STAT_AddValue("Articles Skipped",1);
                    return false; 
                }

                break;                            
            }
            
        // ----------------
        case NA_ONLY_HEADER:
            {

                CheckLinesAndBytesAndGroups(article,killReasons);

                //=========================================
                // Check for the headers to kill or keep
                ExecuteHeaderMatchers(article,killReasons);
                            
                break;
            }

        // ----------------
        case NA_COMPLETE: 
                break;   // There is no need to kill any more 
                         // if we have already downloaded the complete message
    }

    // If a the score is above 0 we keep the message.
    if ( article->Score() < 0)
    {
        killReasons 
            << "Total :" << article->Score() << endl
            << ends << flush; // Terminate the string
        char * cstr_reasons = killReasons.str();
        LogKillEvent(article,cstr_reasons);
        STAT_AddValue("Articles Killed",1);
        delete [] cstr_reasons;// Avoid memory leaks
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------
/*
bool 
NewsKiller::DoesTheHeaderMatch(bool   kill_keep,
                               NEWSArticle * article,
                               string headerName,
                               string headerValue,
                               string &matchedValue)
{
    vector<headerMatchers *> *rules;
    string sectionName;

    if (kill_keep == true)
    {
        if(fKillHeaders.find(headerName) == fKillHeaders.end())
            return false; // No header match

        rules       = &(fKillHeaders[headerName]);
        sectionName = SUCK_KILL_HEADERS;
    }
    else
    {
        if(fKeepHeaders.find(headerName) == fKeepHeaders.end())
            return false; // No header match

        rules       = &(fKeepHeaders[headerName]);
        sectionName = SUCK_KEEP_HEADERS;
    }

    for(vector<HeaderMatcher *>::const_iterator
        ruleIter  = rules->begin();
        ruleIter != rules->end();
        ruleIter ++)
    {
        HeaderMatcher * rule = *ruleIter;



        if (headerValue.find(rule->valueToMatch) != headerValue.npos)
        {   // Found a matching rule
            valuesMutex.lock();
            rule->lastOcurrance = fNow;
            rule->count ++;

            char countstr[50];
            sprintf(countstr,"%ld",rule->count);
            fSettings->SetValue(sectionName,
                                rule->keyName,
                                rule->lastOcurrance + " ; " + countstr);
            valuesMutex.unlock();
            matchedValue = rule->valueToMatch;
            return true;
        }
    }
    
    return false;
}
*/
//-------------------------------------------------------------------------

void
NewsKiller::CheckLinesAndBytesAndGroups(NEWSArticle * article,strstream &killReasons)
{
    //=========================================
    // Check the minimum number of lines limit.
    if (fMinLines > 0 && 
        article->fLines <= (unsigned long)fMinLines)
    {
        killReasons 
            << "[KILL](-" << fMinLinesImpact << "): Number of lines (" << article->fLines 
            << ") is less than the minimum number of lines (" 
            << fMinLines << ")." << endl;
        article->Score(-1*fMinLinesImpact);
    }

    //=========================================
    // Check the maximum number of lines limit.
    if (fMaxLines > 0 && 
        article->fLines > (unsigned long)fMaxLines)
    {
        killReasons 
            << "[KILL](-" << fMaxLinesImpact << "): Number of lines (" << article->fLines 
            << ") exceeds the maximum number of lines (" 
            << fMaxLines << ")." << endl;
        article->Score(-1*fMaxLinesImpact);
    }

    //=========================================
    // Check the maximum number of bytes limit.
    if (fMaxBytes > 0 && 
        article->fBytes > (unsigned long)fMaxBytes)
    {
        killReasons
            << "[KILL](-" << fMaxBytesImpact << "): Number of bytes (" << article->fBytes
            << ") exceeds the maximum number of bytes ("
            << fMaxBytes << ")."<< endl;
        article->Score(-1*fMaxBytesImpact);
    }

    //==========================================
    // Check the maximum number of groups limit.
    if (fMaxGroups > 0 && 
        article->fNrOfNewsGroups > (unsigned long)fMaxGroups)
    {
        killReasons 
            << "[KILL](-" << fMaxGroupsImpact  <<"): Number of groups (" << article->fNrOfNewsGroups 
            << ") exceeds the maximum number of groups (" 
            << fMaxGroups << ")." << endl;
        article->Score(-1*fMaxGroupsImpact);
    }
}

//-------------------------------------------------------------------------

void 
NewsKiller::ExecuteHeaderMatchers(NEWSArticle * article, strstream &killReasons)
{
    string matchedValue;
    string headerValue;
    vector<HeaderMatcher*>::iterator matchIter;

    //=========================================
    // Check for the headers to kill and keep.
    for(vector<string>::const_iterator
        headerIter  = fHeadersToCheck.begin();
        headerIter != fHeadersToCheck.end();
        headerIter ++)
    {
        string headerName = *headerIter;
        
        if (article->GetHeaderField(headerName,headerValue))
        {
            vector<HeaderMatcher*> *rules = NULL;

            if(fKillHeaders.find(headerName) != fKillHeaders.end())
            {
                rules = &(fKillHeaders[headerName]);
                for(matchIter  = rules->begin();
                    matchIter != rules->end();
                    matchIter ++)
                {
                    string logentry;
                    long score = (*matchIter)->GetImpactValue(article,headerName,logentry);
                    if ( score != 0)
                    {
                        article->Score(-1*score);
                        killReasons << "[KILL](-" << score << ") " << logentry << endl;
                    }
                }
            }

            if(fKeepHeaders.find(headerName) != fKeepHeaders.end())
            {
                rules = &(fKeepHeaders[headerName]);
                for(matchIter  = rules->begin();
                    matchIter != rules->end();
                    matchIter ++)
                {
                    string logentry;
                    long score = (*matchIter)->GetImpactValue(article,headerName,logentry);
                    if ( score != 0)
                    {
                        article->Score(score);
                        killReasons << "[KEEP](+" << score << ") " << logentry << endl;
                    }
                }
            }
        }
    }

    //=========================================
    for(matchIter  = fKillOther.begin();
        matchIter != fKillOther.end();
        matchIter ++)
    {
        string logentry;
        long score = (*matchIter)->GetImpactValue(article,"",logentry);
        if ( score != 0)
        {
            article->Score(-1*score);
            killReasons << "[KILL](-" << score << ") " << logentry << endl;
        }
    }

    //=========================================
    for(matchIter  = fKeepOther.begin();
        matchIter != fKeepOther.end();
        matchIter ++)
    {
        string logentry;
        long score = (*matchIter)->GetImpactValue(article,"",logentry);
        if ( score != 0)
        {
            article->Score(score);
            killReasons << "[KEEP](+" << score << ") " << logentry << endl;
        }
    }
}

//-------------------------------------------------------------------------

void 
NewsKiller::LogKillEvent(NEWSArticle *article,
                         const char * reason)
{
    if (fLogKilledMessages && article != NULL)
    {
        omni_mutex_lock lock(fKillLogFileMutex);

        fKillLogFile 
            << "----------------------------------------------------" << endl
            << "Killed message " << article->fArticleNr 
                 << " of group \"" << article->fPrimaryNewsGroup << "\"." << endl
            << reason
            << "-------------" << endl
            << "Message information:" << endl;

        for(vector<string>::const_iterator
            headerIter  = fHeadersToMentionInKillLog.begin();
            headerIter != fHeadersToMentionInKillLog.end();
            headerIter ++)
        {
            string header = *headerIter;
            string value;
            if (article->GetHeaderField(*headerIter,value))
            {
                fKillLogFile << "  " << *headerIter << ": " << value << endl;
            }
        }
            
        fKillLogFile << flush;
    }
}

//-------------------------------------------------------------------------
// Returns 
//  - true  if stored.
//  - false if not stored because it is already present.
bool
NewsKiller::StoreMessageID(string newMessageID)
{
    omni_mutex_lock lock(messageDBmutex);
    // If newMessageID is not yet present then it will return
    // the default value: false
    if (allMessageIDs[newMessageID] == true)
        return false;
    
    allMessageIDs[newMessageID] = true;
    return true;
}

//-------------------------------------------------------------------------
IMPLEMENT_PRINTABLE_OPERATORS(NewsKiller)

void 
NewsKiller::Print (ostream &os) const
{
    os  << "NewsKiller= {" << endl;

    os  << "}" << endl << flush ;
}

//--------------------------------------------------------------------

// End of the file NewsKiller.cpp
//=========================================================================
