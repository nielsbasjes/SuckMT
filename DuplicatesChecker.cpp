//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : DuplicatesChecker.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 21:30:49 $
//  $Revision: 1.2 $
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

//-------------------------------------------------------------------------
// This class reads the specified restart filename from the specified IniFile
// and logs this event by itself when a matching query is performed.
//-------------------------------------------------------------------------

DuplicatesChecker::DuplicatesChecker(IniFile *settings)
    : Printable("DuplicatesChecker")
    , ArticleImpactChecker(settings)
{
    fRestartFileName = "/tmp/suckmt_restart_default.txt";

    // Quick check of the parameters
    if (fSettings != NULL)
    {
        // Get the restartfilename
        if (fSettings->GetValue( SUCK_CONFIG,
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

long
DuplicatesChecker::GetImpactValue(
        NEWSArticle * article,
        const string &headerName,
        string &/*matchlog*/)
{
    if (article == NULL)
        return 0; // Not valid -> no Impact

    if (headerName != "Message-ID")
        return 0; // Wrong header -> no Impact

    string newMessageID = article->fMessageID;

    if (article->GetState() != NA_COMPLETE)
    {   // The article has NOT YET been downloaded
        // Check if this is a duplicate
        omni_mutex_lock lock(fMessageDBmutex);

        // If newMessageID is not yet present then it will return
        // the default value: false
        if (fAllMessageIDs[newMessageID] == true)
            return 1000000; // 1000000 == extreme impact

        // Append the newMessageID to both storage systems
        // The first is for quick checking, the second goes 
        // faster to the restart file.
        fAllMessageIDs[newMessageID] = true;
    }
    else
    {   // The article has been downloaded so we append it to the restart file
        omni_mutex_lock lock(fRestartFileMutex);
        fRestartFile << newMessageID << endl << flush;
    }

    return 0; // 0 == no impact
}

//--------------------------------------------------------------------

// This function returns the name of the header
// for which this ArticleImpactChecker will check.
// Returns true if headerName is filled.
// Returns false if this ArticleImpactChecker will check
// Several headers
bool
DuplicatesChecker::GetMatchingHeader(string & headerName)
{
    headerName = "Message-ID";
    return true;
}

//--------------------------------------------------------------------

IMPLEMENT_PRINTABLE_OPERATORS(DuplicatesChecker);

void 
DuplicatesChecker::Print (ostream &os) const
{
    os  << "DuplicatesChecker= {" << "}" << endl;
}

//-------------------------------------------------------------------------

// End of the file DuplicatesChecker.cpp
//=========================================================================
