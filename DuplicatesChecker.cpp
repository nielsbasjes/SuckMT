//=========================================================================
//                 Copyright (C)1999-2000 by Niels Basjes
//                  SuckMT Website : http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : DuplicatesChecker.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2002/02/18 23:26:09 $
//  $Revision: 1.13 $
//  $RCSfile: DuplicatesChecker.cpp,v $
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

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#if (!defined WIN32 || defined __CYGWIN__)
#include <unistd.h>
#endif

#ifdef __CYGWIN__
#include <unistd.h>
#endif

#include <time.h>
#include <stdlib.h>
#include "SuckDefines.h"
#include "DuplicatesChecker.h"
#include "Tokenize.h"
#include "TraceLog.h"

//-------------------------------------------------------------------------
// This class reads the specified restart filename from the specified IniFile
// and logs this event by itself when a matching query is performed.
//-------------------------------------------------------------------------

#define RESTART_READ_BUFFER 10000

DuplicatesChecker::DuplicatesChecker(IniFile *settings)
{
    fRestartFileName = "/tmp/suckmt_restart_default.txt";

    char buffer[RESTART_READ_BUFFER+2];
    memset(&buffer,0,RESTART_READ_BUFFER+2); 

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
                int count = 0;

                while(!inFile.eof())
                {
                    inFile.getline(buffer,RESTART_READ_BUFFER);
                    string work_buffer(buffer);
                    if (work_buffer.length() > 3)
                    {
                        fAllMessageIDs[work_buffer] = true;
                        count++;
                    }
                }

                // Create a duplicate copy
                fRestartMessageIDs = fAllMessageIDs;

                if (count > 0)
                {
                    Linfo << "Loaded " << count 
                          << " message-IDs from the restart file." 
                          << endl << flush;
                }
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
    if (fRestartMessageIDs[article->fMessageID] == true)
        return; // We have this one -> don't append to restart file

    // Store the newMessageID 
    fRestartMessageIDs[article->fMessageID] = true;
    
    fRestartFile << article->fMessageID << endl << flush;
}

//-------------------------------------------------------------------------

void
DuplicatesChecker::ArticleHasBeenKilled(NEWSArticle * article)
{   
    // The article has been killed so we append it to the restart file
    omni_mutex_lock lock(fRestartFileMutex);
    fRestartFile << article->fMessageID << endl << flush;
}

//-------------------------------------------------------------------------

// End of the file DuplicatesChecker.cpp
//=========================================================================
