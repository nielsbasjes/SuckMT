//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : DuplicatesChecker.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/19 12:21:38 $
//  $Revision: 1.3 $
//  $RCSfile: DuplicatesChecker.cpp,v $
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

#include <time.h>
#include "SuckDefines.h"
#include "DuplicatesChecker.h"
#include "Tokenize.h"
#include "TraceLog.h"
#include "StatisticsKeeper.h"

//-------------------------------------------------------------------------
// This class reads the specified restart filename from the specified IniFile
// and logs this event by itself when a matching query is performed.
//-------------------------------------------------------------------------

DuplicatesChecker::DuplicatesChecker(IniFile *settings)
{
    fRestartFileName = "/tmp/suckmt_restart_default.txt";

    // Quick check of the parameters
    if (settings != NULL)
    {
        // Get the restartfilename
        if (settings->GetValue( SUCK_CONFIG,
                SUCK_RESTART_FILE, fRestartFileName))
        {
            // Open/Create the file
            ifstream inFile(fRestartFileName.c_str());

            if (inFile.good())
            {
                char buffer[10000];
                int count = 0;

                while(!inFile.eof())
                {
                    inFile.getline(buffer,9999);
                    string work_buffer(buffer);
                    fAllMessageIDs[work_buffer] = true;
                    count++;
                }
                Linfo << "Loaded " << count 
                      << " message-IDs from the restart file." 
                      << endl << flush;
            }
            inFile.close();
        }
    }

    fRestartFile.open(fRestartFileName.c_str(),ios::app);
    if (!fRestartFile.good())
        return;
}

//-------------------------------------------------------------------------

DuplicatesChecker::~DuplicatesChecker()
{
    omni_mutex_lock lock(fRestartFileMutex);
    fRestartFile.close();

    if (KeepRunning())
    {   // We will ONLY get here iff 
        // 1) The download has stopped.
        // 2) The application did NOT receive an Abort signal
        // This means that the donwload completed succesfully
        // Delete the restart file
        unlink(fRestartFileName.c_str());
    }
}

//-------------------------------------------------------------------------

bool
DuplicatesChecker::DoWeNeedToDownloadThisArticle(NEWSArticle * article)
{
    if (article == NULL)
        return false; // Not valid -> don't download

    string newMessageID = article->fMessageID;

    // Check if this is a duplicate
    omni_mutex_lock lock(fMessageDBmutex);

    // If newMessageID is not yet present then it will return
    // the default value: false
    if (fAllMessageIDs[newMessageID] == true)
        return false; // We have this one -> don't download

    // Store the newMessageID 
    fAllMessageIDs[newMessageID] = true;

    return true; // we want to download
}

//-------------------------------------------------------------------------

void
DuplicatesChecker::ArticleHasBeenStored(NEWSArticle * article)
{   
    // The article has been downloaded so we append it to the restart file
    omni_mutex_lock lock(fRestartFileMutex);
    fRestartFile << article->fMessageID << endl << flush;
    STAT_AddValue("Articles Written",1);
}

//-------------------------------------------------------------------------

void
DuplicatesChecker::ArticleHasBeenKilled(NEWSArticle * article)
{   
    // The article has been killed so we append it to the restart file
    omni_mutex_lock lock(fRestartFileMutex);
    fRestartFile << article->fMessageID << endl << flush;
    STAT_AddValue("Articles Killed",1);
}

//-------------------------------------------------------------------------

// End of the file DuplicatesChecker.cpp
//=========================================================================
