//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/10/07 19:43:12 $
//  $Revision: 1.8 $
//  $RCSfile: NewsKiller.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#define TRY_ALL_KILL_MATCHES 1

//-------------------------------------------------------------------------

#include <algorithm>
#include <time.h>
#include "omnithread.h"
#include "SuckDefines.h"
#include "NewsKiller.h"
#include "Tokenize.h"
#include "StatisticsKeeper.h"

//-------------------------------------------------------------------------

// With   strstream foo;
// Doing "foo << endl" or "foo << ends" in Visual C++ 6.0 results in foo 
// getting the address of the function appended ...... sigh.
//#ifdef __WIN32__
//#define  endl  "\n"
//#define  ends  char('\0')
//#endif

//-------------------------------------------------------------------------

static bool 
StringToBool(string value)
{
    if (value == "1"    || 
        value == "TRUE" ||
        value == "True" ||
        value == "true" ||
        value == "YES"  ||
        value == "Yes"  ||
        value == "yes"  )
        return true;
    return false;
}
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
    string logKilledMessages;

    if (fSettings != NULL)
    {
        //=====================

        if (fSettings->GetValue(SUCK_KILL_LOGFILE,SUCK_KILL_ENABLE_LOGFILE,logKilledMessages))
        {
            fLogKilledMessages = StringToBool(logKilledMessages);

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

        //=====================

        ReadHeaderRules(SUCK_KILL_HEADERS,fHeadersToCheck,fKillHeaders);
        ReadHeaderRules(SUCK_KEEP_HEADERS,fHeadersToCheck,fKeepHeaders);
    }
    
//cout << this;
    
}


//-------------------------------------------------------------------------

void
NewsKiller::ReadHeaderRules
    (string                                     iniSection,
     vector<string>                            &headersToCheck,
     map<string,vector<headerMatchStruct*> >   &headerRules)
{
    list<string> variableNames;
    if (fSettings->GetVariableNames(iniSection,variableNames))
    {
        for(list<string>::const_iterator
            itemIter  = variableNames.begin();
            itemIter != variableNames.end();
            itemIter ++)
        {
            string thisRule = (*itemIter);
            unsigned long position = thisRule.find(":");

            if (position != thisRule.npos)
            {   // We have a legal kill line
                headerMatchStruct * rule = new headerMatchStruct();

                rule->keyName = thisRule;
                rule->count = 0;

                string fieldName(thisRule,0,position);
                rule->valueToMatch.assign(thisRule.begin() + position + 1,thisRule.end());

                string ruleValue;
                if (!fSettings->GetValue(iniSection,thisRule,ruleValue))
                {
                    cerr << "STRANGE ERROR" << endl;
                    continue;
                }

                if (ruleValue=="")
                {
//                    string value = "New rule --- " + fNow + " --- ";
                    string value = fNow + " ; 0 ";
                    fSettings->SetValue(iniSection,thisRule,value);
                }
                else
                {
                    unsigned long position = ruleValue.find(";");

                    if (position != ruleValue.npos)
                    {
                        rule->lastOcurrance.assign(ruleValue,0,position);

                        string count(ruleValue.begin() + position + 1,ruleValue.end());

                        const char * beginptr = count.c_str();
                        char       * endptr   = NULL;

                        if (*beginptr != 0x00) 
                        {   // The existing value is not an empty string
                            long currentNumberValue = 
                                    strtol(count.c_str(),&endptr, 10);

                            //  man strtol says: "Thus, if *nptr is not `\0' but **endptr 
                            //  is `\0' on return, the entire string is valid.
                            if (*endptr == 0x00 )
                                rule->count = currentNumberValue;
                        }
                    }
                }

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
        }
    }
}

//-------------------------------------------------------------------------

NewsKiller::~NewsKiller()
{
    EraseHeaderRules(fKillHeaders);
    EraseHeaderRules(fKeepHeaders);
    
    fKillLogFile 
        << "----------------------------------------------------" << endl
        << "End of run started at : " << fNow << endl << flush
        << "====================================================" << endl << flush;
}

//-------------------------------------------------------------------------

void
NewsKiller::EraseHeaderRules(map<string,vector<headerMatchStruct*> > &headerRules)
{
    for(map<string,vector<headerMatchStruct*> >::iterator
        mapIter  = headerRules.begin();
        mapIter != headerRules.end();
        mapIter ++)
    {
        vector<headerMatchStruct*> * headerStructs = &(mapIter->second);

        for(vector<headerMatchStruct*>::iterator
            headerIter  = headerStructs->begin();
            headerIter != headerStructs->end();
            headerIter ++)
        {
            delete *headerIter;
        }
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
    string matchedValue;

    strstream killReasons; 

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
                    return false; // We don't want this logged so we jump out here
                }

                break;                            
            }
            
        // ----------------
        case NA_ONLY_HEADER:
            {

                CheckLinesAndBytesAndGroups(article,killReasons);

#ifndef TRY_ALL_KILL_MATCHES
                if (article->KeepMatched())
                    break
#endif
                //=========================================
                // Check for the headers to kill or keep
                MatchHeaders(article,killReasons);
                            
                break;
            }

        // ----------------
        case NA_COMPLETE: 
                break;   // There is no need to kill any more 
                         // if we have already downloaded the complete message
    }

    // If a kill matched and NO keep the kill.
    if ( article->KillMatched() && 
        !article->KeepMatched())
    {
        killReasons << ends << flush; // Terminate the string
        char * cstr_reasons = killReasons.str();
        LogKillEvent(article,cstr_reasons);
        STAT_AddValue("Articles Killed",1);
        delete [] cstr_reasons;// Avoid memory leaks
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------
bool 
NewsKiller::DoesTheHeaderMatch(bool   kill_keep,
                               string headerName,
                               string headerValue,
                               string &matchedValue)
{
    vector<headerMatchStruct *> *rules;
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

    for(vector<headerMatchStruct *>::const_iterator
        ruleIter  = rules->begin();
        ruleIter != rules->end();
        ruleIter ++)
    {
        headerMatchStruct * rule = *ruleIter;
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

//-------------------------------------------------------------------------

void 
NewsKiller::CheckLinesAndBytesAndGroups(NEWSArticle * article,strstream &killReasons)
{
    //=========================================
    // Check the maximum number of lines limit.
    long maxLines;
    if (fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,
                            SUCK_MAX_LINES,
                            maxLines))
    {   // Maxlines has been set
        if (maxLines > 0 && 
            article->fLines > (unsigned long)maxLines)
        {
            killReasons 
                << "KILLED FOR: Number of lines (" << article->fLines 
                << ") exceeds the maximum number of lines (" 
                << maxLines << ")." << endl;
            article->KillMatched(true);

#ifndef TRY_ALL_KILL_MATCHES
            return;
#endif
        
        }
    }

    //=========================================
    // Check the maximum number of bytes limit.
    long maxBytes;
    if (fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,
                            SUCK_MAX_BYTES,
                            maxBytes))
    {   // maxBytes has been set
        if (maxBytes > 0 && 
            article->fBytes > (unsigned long)maxBytes)
        {
            killReasons
                << "KILLED FOR: Number of bytes (" << article->fBytes
                << ") exceeds the maximum number of bytes ("
                << maxBytes << ")."<< endl;
            article->KillMatched(true);
#ifndef TRY_ALL_KILL_MATCHES
            return;
#endif
        }
    }

    //==========================================
    // Check the maximum number of groups limit.
    long maxGroups;
    if (fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,
                            SUCK_MAX_GROUPS,
                            maxGroups))
    {   // maxBytes has been set
        if (maxGroups > 0 && 
            article->fNrOfNewsGroups > (unsigned long)maxGroups)
        {
            killReasons 
                << "KILLED FOR: Number of groups (" << article->fNrOfNewsGroups 
                << ") exceeds the maximum number of groups (" 
                << maxGroups << ")." << endl;
            article->KillMatched(true);
#ifndef TRY_ALL_KILL_MATCHES
            return;
#endif
        }
    }
}

//-------------------------------------------------------------------------

void 
NewsKiller::MatchHeaders(NEWSArticle * article, strstream &killReasons)
{
    string matchedValue;

    //=========================================
    // Check for the headers to kill and keep.
    for(vector<string>::const_iterator
        headerIter  = fHeadersToCheck.begin();
        headerIter != fHeadersToCheck.end();
        headerIter ++)
    {
        MatchOneHeader(article, killReasons, *headerIter);

#ifndef TRY_ALL_KILL_MATCHES
        if (article->KeepMatched())
            return;
#endif
    }
}

//-------------------------------------------------------------------------

void 
NewsKiller::MatchOneHeader(NEWSArticle * article, strstream &killReasons ,string headerName)
{
    string matchedValue;
    string headerValue;
    if (article->GetHeaderField(headerName,headerValue))
    {
        if (DoesTheHeaderMatch(USE_KEEP,headerName,headerValue,matchedValue))
        {
            article->KeepMatched(true);
#ifndef TRY_ALL_KILL_MATCHES
            return;
#endif
        }

        if (DoesTheHeaderMatch(USE_KILL,headerName,headerValue,matchedValue))
        {
            killReasons 
                << "KILLED FOR: The header \"" << headerName 
                << ":" << headerValue << "\" contains the string \"" 
                << matchedValue << "\"." << endl;
            article->KillMatched(true);
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

/*    for(vector<string>::const_iterator
        headerIter  = fHeadersToCheck.begin();
        headerIter != fHeadersToCheck.end();
        headerIter ++)
    {
        string fieldName = *headerIter;
//      vector<headerMatchStruct *> killers = fKillHeaders[fieldName];
        map<string,vector<headerMatchStruct *> >::const_iterator killers = 
                fKillHeaders.find(fieldName);
        os << "######" << fieldName << "######" << endl;
        
        for(vector<headerMatchStruct *>::const_iterator
            killIter  = killers->second.begin();
            killIter != killers->second.end();
            killIter ++)
        {
            headerMatchStruct * killer = *killIter;
            os << killer->keyName << "(==\""<<fieldName<<"\")"<< "\t" 
               << killer->valueToMatch << "\t" 
               << killer->lastOcurrance << "\t"
               << killer->count << endl;
        }
    }    
*/
    os  << "}" << endl << flush ;
}

//--------------------------------------------------------------------

// End of the file NewsKiller.cpp
//=========================================================================
