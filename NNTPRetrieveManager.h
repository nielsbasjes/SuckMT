/***************************************************************************
                          NNTPRetrieveManager.h  -  description                              
                             -------------------                                         
    begin                : Mon Jul 26 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifndef NNTPRETRIEVEMANAGER_H
#define NNTPRETRIEVEMANAGER_H

class NNTPRetrieveManager; // Forward Declaration

#include <vector>
#include <string>
#include "omnithread.h"
#include "Abortable.h"
#include "IniFile.h"
#include "CommandQueue.h"
#include "NNTPCommandHandler.h"
#include "NewsKiller.h"

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#endif


class NNTPRetrieveManager : public Abortable
{
public: 
	NNTPRetrieveManager(IniFile &settings);
	~NNTPRetrieveManager();

    // Get the messages from the specified groups
    void
    GetGroups(vector<string> groupNames);

    // Store th specified messageID so we can check if we get any duplicates
    // Returns 
    //  - true  if the messageID was unknown and it has been stored.
    //  - false if the messageID was already present and it has not been stored.
    bool
    StoreMessageID(string newMessageID);

    // Store the specified filename so we know which files have to 
    // be processed later
    void
    ArticleFileHasBeenWritten(string fileName);

    void
    WaitForCompletion ();

    void 
    AbortChildren();

private:
    omni_mutex nrm_mutex;
    
    vector <NNTPCommandHandler*> NNTPHandlers;
    
    vector <string> fFilesWritten;
    
    CommandQueue  commands;
    
    NewsKiller   fKiller;
    IniFile      *fSettings;
};

#endif
