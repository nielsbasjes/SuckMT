//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPRetrieveManager.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:53:20 $
//  $Revision: 1.4 $
//  $RCSfile: NNTPRetrieveManager.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __NNTPRETRIEVEMANAGER_H__
#define __NNTPRETRIEVEMANAGER_H__

//-------------------------------------------------------------------------

class NNTPRetrieveManager; // Forward Declaration

//-------------------------------------------------------------------------

#include <vector>
#include <string>
#include "omnithread.h"
#include "Abortable.h"
#include "IniFile.h"
#include "CommandQueue.h"
#include "NNTPCommandHandler.h"
#include "NewsKiller.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string std::string
#endif

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

#endif // __NNTPRETRIEVEMANAGER_H__

// End of the file NNTPRetrieveManager.h
//=========================================================================
