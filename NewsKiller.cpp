//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NewsKiller.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/30 17:34:59 $
//  $Revision: 1.4 $
//  $RCSfile: NewsKiller.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include <time.h>
#include <string>
#include "omnithread.h"
#include "SuckDefines.h"
#include "NewsKiller.h"
#include "StatisticsKeeper.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#endif

//-------------------------------------------------------------------------

NewsKiller::NewsKiller(IniFile * settings)
{
    time_t now = time(NULL);
    char * nowStr = ctime(&now);
    nowStr[24] = 0x00; // Remove the endl symbol from the time string

    fNow = nowStr;

    fSettings = settings;
    list<string> killVariableNames;

    if (fSettings != NULL && 
        fSettings->GetVariableNames(SUCK_KILL_HEADERS,killVariableNames))
    {
        for(list<string>::const_iterator
            itemIter  = killVariableNames.begin();
            itemIter != killVariableNames.end();
            itemIter ++)
        {
            string thisKiller = (*itemIter);
            unsigned long position = thisKiller.find(":");

            if (position != thisKiller.npos)
            {   // We have a legal kill line
                killStruct * killer = new killStruct();

                killer->keyName = thisKiller;

                string fieldName(thisKiller,0,position);
                killer->valueToKill.assign(thisKiller,position+1);

                string killValue;
                if (!fSettings->GetValue(SUCK_KILL_HEADERS,thisKiller,killValue))
                {
                    cerr << "STRANGE ERROR" << endl;
                    continue;
                }

                if (killValue=="")
                {
                    killer->count = 0;
                }
                else
                {
                    unsigned long position = killValue.find(";");

                    if (position != killValue.npos)
                    {
                        killer->lastOcurrance.assign(killValue,0,position);
                        string count(killValue,position+1);
                        killer->count = 0;

                        const char * beginptr = count.c_str();
                        char       * endptr   = NULL;

                        if (*beginptr != 0x00) 
                        {   // The existing value is not an empty string
                            long currentNumberValue = 
                                    strtol(count.c_str(),&endptr, 10);

                            //  man strtol says: "Thus, if *nptr is not `\0' but **endptr 
                            //  is `\0' on return, the entire string is valid.
                            if (*endptr == 0x00 )
                                killer->count = currentNumberValue;
                        }
                    }
                }

                // Store the field kill rule
                if (fKillHeaders.find(fieldName) == fKillHeaders.end())
                {   // This is a new field
                    fHeadersToCheck.push_back(fieldName);
                }

                fKillHeaders[fieldName].push_back(killer);
            }
        }
    }
    
//cout << this;
    
}

//-------------------------------------------------------------------------

NewsKiller::~NewsKiller()
{
    // Nothing to do
}

//-------------------------------------------------------------------------
// The main routine of the Kill processor.
// Returns 
//  - true  if we should continue with this article.
//  - false if we should kill this article.
bool 
NewsKiller::DoWeKeepThisArticle(NEWSArticle * article)
{
    if (article == NULL)
        return false;

    switch (article->GetState())
    {
        // ----------------
        case NA_NOTHING:
            return true; // There is nothing to check yet
            
        // ----------------
        case NA_ONLY_XOVER:
            {
                // Check if this article isn't already downloaded from
                // an other newsgroup
                if (!StoreMessageID(article->fMessageID))
                {
                    STAT_AddValue("Articles Skipped",1);
                    //cout << "SKIPPED DUPLICATE" << endl;
                    return false;
                }

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
//cout << endl << "KILLED maxLines" << endl << flush;
                        STAT_AddValue("Articles Killed",1);
                        return false;
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
//cout << endl << "KILLED maxBytes" << endl << flush;
                        STAT_AddValue("Articles Killed",1);
                        return false;
                    }
                }

                //==================================================
                // Check for From lines to be killed
                if (!DoWeKeepThisArticle("From",article->fSender))
                {
//cout << endl << "KILLED From:" << article->fSender << endl << flush;
                    return false;
                }

                //==================================================
                // Check for From lines to be killed
                if (!DoWeKeepThisArticle("Subject",article->fSubject))
                {
//cout << endl << "KILLED Subject:" << article->fSubject << endl << flush;
                    return false;
                }
                                
                return true;
            }
            
        // ----------------
        case NA_ONLY_HEADER:
            {
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
//cout << endl << "KILLED maxGroups" << endl << flush;
                        STAT_AddValue("Articles Killed",1);
                        return false;
                    }
                }
                
                //=========================================
                // Check for the headers to kill .
                for(vector<string>::const_iterator
                    headerIter  = fHeadersToCheck.begin();
                    headerIter != fHeadersToCheck.end();
                    headerIter ++)
                {
                    string headerName = *headerIter;
                    
                    if (headerName == "From"    ||
                        headerName == "Subject")
                    {
//cout << endl << "OPTIMIZED SKIP "<< headerName<< endl << flush;
                        continue; // These two headers have already been checked.
                    }
                    
                    string headerValue;
                    if (article->GetHeaderField(headerName,headerValue))
                    {
                        if (!DoWeKeepThisArticle(headerName,headerValue))
                            return false;
                    }
                }
                return true;
            }

        // ----------------
        case NA_COMPLETE: 
            return true; // There is no need to kill any more 
                         // if we have already downloaded the complete message
    }
    
    return true;
}

//-------------------------------------------------------------------------
bool 
NewsKiller::DoWeKeepThisArticle(string headerName,string headerValue)
{
    vector<killStruct *> killers = fKillHeaders[headerName];

    for(vector<killStruct *>::const_iterator
        killIter  = killers.begin();
        killIter != killers.end();
        killIter ++)
    {
        killStruct * killer = *killIter;
        if (headerValue.find(killer->valueToKill) != headerValue.npos)
        {   // Found a matching kill entry
            valuesMutex.lock();
            killer->lastOcurrance = fNow;
            killer->count ++;

            char countstr[50];
            sprintf(countstr,"%ld",killer->count);
            fSettings->SetValue(SUCK_KILL_HEADERS,
                                killer->keyName,
                                killer->lastOcurrance + " ; " + countstr);
            valuesMutex.unlock();
cout << endl << "KILLED " << headerName << ":" << headerValue << endl << flush;
            STAT_AddValue("Articles Killed",1);
            return false;
        }
    }
    
    return true;
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

    for(vector<string>::const_iterator
        headerIter  = fHeadersToCheck.begin();
        headerIter != fHeadersToCheck.end();
        headerIter ++)
    {
        string fieldName = *headerIter;
        vector<killStruct *> killers = fKillHeaders[fieldName];
        os << "######" << fieldName << "######" << endl;
        
        for(vector<killStruct *>::const_iterator
            killIter  = killers.begin();
            killIter != killers.end();
            killIter ++)
        {
            killStruct * killer = *killIter;
            os << killer->keyName << "(==\""<<fieldName<<"\")"<< "\t" 
               << killer->valueToKill << "\t" 
               << killer->lastOcurrance << "\t"
               << killer->count << endl;
        }
    }    
    os  << "}" << endl << flush ;
}

//--------------------------------------------------------------------

// End of the file NewsKiller.cpp
//=========================================================================
