/***************************************************************************
                          NewsKiller.cpp  -  description                              
                             -------------------                                         
    begin                : Thu Jul 29 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include <string>
#include "omnithread.h"
#include "SuckDefines.h"
#include "NewsKiller.h"
#include "StatisticsKeeper.h"

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#endif

//-------------------------------------------------------------------------

FUNCTION_START(NewsKiller::NewsKiller(IniFile * settings))
{
    fSettings = settings;
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(NewsKiller::~NewsKiller())
{
	// Nothing to do
}
FUNCTION_END

//-------------------------------------------------------------------------
// Returns 
//  - true  if we should continue with this article.
//  - false if we should kill this article.
bool 
FUNCTION_START(NewsKiller::DoWeKeepThisArticle(NEWSArticle * article))
{
    if (article == NULL)
        return false;
    
    // Is Message ID unique ?
    if (article->header=="" && 
        article->body=="")
    {
        if (!StoreMessageID(article->messageID))
        {
			STAT_AddValue("Articles Killed",1);
            //cout << "KILLED DUPLICATE" << endl;
            return false;
        }
		
        long maxLines;
        if (fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,SUCK_MAX_LINES,maxLines))
        {   // Maxlines has been set
            if (maxLines > 0 && article->lines > maxLines)
			{
				STAT_AddValue("Articles Killed",1);
                return false;
			}
        }
    }
	
    return true;
}
FUNCTION_END

//-------------------------------------------------------------------------
// Returns 
//  - true  if stored.
//  - false if not stored because it is already present.
bool
FUNCTION_START(NewsKiller::StoreMessageID(string newMessageID))
{
    omni_mutex_lock lock(messageDBmutex);
    // If newMessageID is not yet present then it will return
    // the default value: false
    if (allMessageIDs[newMessageID] == true)
        return false;
	
    allMessageIDs[newMessageID] = true;
    return true;
}
FUNCTION_END

//-------------------------------------------------------------------------
