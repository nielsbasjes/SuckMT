//=========================================================================
//                 Copyright (C)1999-2003 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT/
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPRetrieveManager.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2003/04/13 20:51:55 $
//  $Revision: 1.10 $
//  $RCSfile: NNTPRetrieveManager.h,v $
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

#ifndef __NNTPRETRIEVEMANAGER_H__
#define __NNTPRETRIEVEMANAGER_H__

//-------------------------------------------------------------------------

class NNTPRetrieveManager; // Forward Declaration

//-------------------------------------------------------------------------

#include <vector>
#include <string>
#include <omnithread.h>
#include "Abortable.h"
#include "IniFile.h"
#include "CommandQueue.h"
#include "NNTPCommandHandler.h"
#include "NewsKiller.h"
#include "DuplicatesChecker.h"

//-------------------------------------------------------------------------

using namespace std;

//-------------------------------------------------------------------------

class NNTPRetrieveManager : public Abortable
{
public: 
    NNTPRetrieveManager(IniFile &settings);
    ~NNTPRetrieveManager();

    // Get the messages from the specified groups
    void
    GetGroups(vector<string> groupNames);

    // Store the specified messageID so we can check if we get any duplicates
    // Returns 
    //  - true  if the messageID was unknown and it has been stored.
    //  - false if the messageID was already present and it has not been stored.
    bool
    DoWeNeedToDownloadThisArticle(NEWSArticle * article);

    // Make a note this article has been stored in the specified file
    void
    ArticleHasBeenStored(NEWSArticle * article,string filename);

    // Make a note this article has been stored
    void
    ArticleHasBeenKilled(NEWSArticle * article);

    void
    WaitForCompletion ();

    void 
    AbortChildren();

private:

    bool
    CreateHandlers();

    void
    AddGroupsToCommandQueue(list<string> &groups);

    omni_mutex nrm_mutex;
    
    vector <NNTPCommandHandler*> NNTPHandlers;
    
    vector <string> fFilesWritten;
    
    CommandQueue  commands;
    
    NewsKiller         fKiller;
    DuplicatesChecker  fDuplicatesChecker;
    IniFile           *fSettings;
};

//-------------------------------------------------------------------------

#endif // __NNTPRETRIEVEMANAGER_H__

// End of the file NNTPRetrieveManager.h
//=========================================================================
