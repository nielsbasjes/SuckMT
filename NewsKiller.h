/***************************************************************************
                          NewsKiller.h  -  description                              
                             -------------------                                         
    begin                : Thu Jul 29 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef NEWSKILLER_H
#define NEWSKILLER_H

class NewsKiller; // Forward declaration

#include <map>
#include <string>
#include "NEWSArticle.h"
#include "IniFile.h"

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define map    std::map
#define string std::string
#endif

class NewsKiller 
{
public: 
	NewsKiller(IniFile *settings);
	~NewsKiller();

    bool 
    DoWeKeepThisArticle(NEWSArticle * article);

private:
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
};

#endif
